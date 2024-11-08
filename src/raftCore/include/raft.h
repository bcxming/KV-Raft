#ifndef RAFT_H
#define RAFT_H

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "ApplyMsg.h"
#include "Persister.h"
#include "boost/any.hpp"
#include "boost/serialization/serialization.hpp"
#include "config.h"
#include "raftRpcUtil.h"
#include "util.h"

// constexpr用于定义一个编译时常量
// 方便网络分区的时候debug，网络异常的时候为Disconnected，只要网络正常就为AppNormal，防止matchIndex[]数组异常减小
constexpr int Disconnected = 0;  // 表示网络断开
constexpr int AppNormal = 1;     // 表示网络正常

// 投票状态
constexpr int Killed = 0;    // 表示节点被“杀死”或关闭
constexpr int Voted = 1;     // 本轮已经投过票
constexpr int Expire = 2;    // 投票（消息、竞选者）过期
constexpr int Normal = 3;    // 正常状态

// Raft 类的声明
class Raft : public raftRpcProctoc::raftRpc {
 private:
  std::mutex m_mtx;  // 用于线程同步的互斥锁，保护共享数据
  std::vector<std::shared_ptr<RaftRpcUtil>> m_peers;  // 存储其他节点的 RPC 工具
  std::shared_ptr<Persister> m_persister;  // 持久化存储器，保存状态和日志
  int m_me;  // 当前节点的 ID
  int m_currentTerm;  // 当前任期
  int m_votedFor;  // 当前投票支持的候选者 ID
  // 日志条目数组，包含了状态机要执行的指令集，以及收到领导时的任期号
  std::vector<raftRpcProctoc::LogEntry> m_logs;
  // 这两个状态所有结点都在维护，易失
  int m_commitIndex;  // 已提交的日志条目的最高索引
  int m_lastApplied;  // 已经应用到状态机的日志条目索引

  // 这些状态由服务器维护
  // 下标从 1 开始，避免与 commitIndex 和 lastApplied 混淆
  std::vector<int> m_nextIndex;  // 每个节点的下一个日志条目索引
  std::vector<int> m_matchIndex;  // 每个节点的匹配日志条目索引
  enum Status { Follower, Candidate, Leader };  // 节点的角色状态
  Status m_status;  // 当前节点的身份

  std::shared_ptr<LockQueue<ApplyMsg>> applyChan;  // 与客户端通信的队列，传递应用的日志

  // 选举超时
  std::chrono::_V2::system_clock::time_point m_lastResetElectionTime;  // 上次重置选举定时器的时间
  // 心跳超时，用于领导者发送心跳
  std::chrono::_V2::system_clock::time_point m_lastResetHearBeatTime;  // 上次重置心跳定时器的时间

  // 用于保存快照中的最后一个日志的索引和任期
  int m_lastSnapshotIncludeIndex;
  int m_lastSnapshotIncludeTerm;

 public:
  // Raft 的各类方法声明
  // 日志同步 + 心跳 rpc
  void AppendEntries1(const raftRpcProctoc::AppendEntriesArgs *args, raftRpcProctoc::AppendEntriesReply *reply);  
  void applierTicker();  // 用于将日志条目应用到状态机的定时器

  // 安装快照
  bool CondInstallSnapshot(int lastIncludedTerm, int lastIncludedIndex, std::string snapshot); 
  void doElection();  // 发起选举
  void doHeartBeat();  // 发送心跳

  void electionTimeOutTicker();  // 选举超时定时器
  std::vector<ApplyMsg> getApplyLogs();  // 获取应用的日志
  int getNewCommandIndex();  // 获取新命令的日志索引
  void getPrevLogInfo(int server, int *preIndex, int *preTerm);  // 获取前一个日志条目的信息
  void GetState(int *term, bool *isLeader);  // 获取当前状态
  void InstallSnapshot(const raftRpcProctoc::InstallSnapshotRequest *args,
                       raftRpcProctoc::InstallSnapshotResponse *reply);  // 安装快照的 RPC
  void leaderHearBeatTicker();  // 领导者心跳定时器
  void leaderSendSnapShot(int server);  // 领导者发送快照
  void leaderUpdateCommitIndex();  // 领导者更新提交索引

  bool matchLog(int logIndex, int logTerm);  // 检查日志条目是否匹配
  void persist();  // 持久化当前状态
  
  void RequestVote(const raftRpcProctoc::RequestVoteArgs *args, raftRpcProctoc::RequestVoteReply *reply);  // 处理投票请求
  bool UpToDate(int index, int term);  // 判断日志是否是最新的
  int getLastLogIndex();  // 获取最后一个日志条目的索引
  int getLastLogTerm();  // 获取最后一个日志条目的任期
  void getLastLogIndexAndTerm(int *lastLogIndex, int *lastLogTerm);  // 获取最后一个日志条目的索引和任期
  int getLogTermFromLogIndex(int logIndex);  // 获取指定日志条目的任期
  int GetRaftStateSize();  // 获取当前 Raft 状态的大小
  int getSlicesIndexFromLogIndex(int logIndex);  // 根据日志索引获取切片索引

  bool sendRequestVote(int server, std::shared_ptr<raftRpcProctoc::RequestVoteArgs> args,
                       std::shared_ptr<raftRpcProctoc::RequestVoteReply> reply, std::shared_ptr<int> votedNum);  // 发送投票请求
  bool sendAppendEntries(int server, std::shared_ptr<raftRpcProctoc::AppendEntriesArgs> args,
                         std::shared_ptr<raftRpcProctoc::AppendEntriesReply> reply, std::shared_ptr<int> appendNums);  // 发送追加日志请求

  void pushMsgToKvServer(ApplyMsg msg);  // 将消息推送到 KV 服务器
  void readPersist(std::string data);  // 从持久化存储中读取状态
  std::string persistData();  // 持久化当前数据

  void Start(Op command, int *newLogIndex, int *newLogTerm, bool *isLeader);  // 开始处理命令

  // 快照相关方法
  void Snapshot(int index, std::string snapshot);  // 生成快照

 public:
  // 重写基类方法，处理 RPC 调用
  void AppendEntries(google::protobuf::RpcController *controller, const ::raftRpcProctoc::AppendEntriesArgs *request,
                     ::raftRpcProctoc::AppendEntriesReply *response, ::google::protobuf::Closure *done) override;
  void InstallSnapshot(google::protobuf::RpcController *controller,
                       const ::raftRpcProctoc::InstallSnapshotRequest *request,
                       ::raftRpcProctoc::InstallSnapshotResponse *response, ::google::protobuf::Closure *done) override;
  void RequestVote(google::protobuf::RpcController *controller, const ::raftRpcProctoc::RequestVoteArgs *request,
                   ::raftRpcProctoc::RequestVoteReply *response, ::google::protobuf::Closure *done) override;

 public:
  // 初始化 Raft
  void init(std::vector<std::shared_ptr<RaftRpcUtil>> peers, int me, std::shared_ptr<Persister> persister,
            std::shared_ptr<LockQueue<ApplyMsg>> applyCh);

 private:
  // Raft 节点的持久化类
  class BoostPersistRaftNode {
   public:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version) {
      ar &m_currentTerm;
      ar &m_votedFor;
      ar &m_lastSnapshotIncludeIndex;
      ar &m_lastSnapshotIncludeTerm;
      ar &m_logs;
    }
    int m_currentTerm;  // 当前任期
    int m_votedFor;  // 投票支持的候选者
    int m_lastSnapshotIncludeIndex;  // 快照包含的最后一个日志索引
    int m_lastSnapshotIncludeTerm;  // 快照包含的最后一个日志任期
    std::vector<std::string> m_logs;  // 日志条目
    std::unordered_map<std::string, int> umap;  // 其他状态信息
  };
};

#endif  // RAFT_H
