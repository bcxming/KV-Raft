#ifndef SKIP_LIST_ON_RAFT_KVSERVER_H
#define SKIP_LIST_ON_RAFT_KVSERVER_H

// 引入所需的 Boost 库，用于序列化和反序列化、并发控制以及容器类型
#include <boost/any.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include "kvServerRPC.pb.h"  // 包含 kvServerRPC 协议的定义
#include "raft.h"  // 包含 Raft 协议实现的头文件
#include "skipList.h"  // 包含跳表的实现

// KvServer 类继承自 raftKVRpcProctoc::kvServerRpc，用于处理远程客户端的请求
class KvServer : raftKVRpcProctoc::kvServerRpc {
 private:
  std::mutex m_mtx;  // 互斥锁，用于保证线程安全
  int m_me;  // 当前服务器的唯一标识符
  std::shared_ptr<Raft> m_raftNode;  // Raft 节点对象，处理分布式一致性算法
  std::shared_ptr<LockQueue<ApplyMsg> > applyChan;  // KvServer 和 Raft 节点的通信管道，用于接收 Raft 日志应用消息
  int m_maxRaftState;  // 当 Raft 日志达到一定大小时，触发生成快照操作

  std::string m_serializedKVData;  // 序列化后的 KV 数据，目前没有找到替代方案来保存快照数据
  SkipList<std::string, std::string> m_skipList;  // 使用跳表存储键值对
  std::unordered_map<std::string, std::string> m_kvDB;  // 键值数据库

  // 等待应用的操作映射表，用于处理 Raft 日志应用
  std::unordered_map<int, LockQueue<Op> *> waitApplyCh;

  // 客户端的最后请求 ID 映射，用于检查请求是否重复
  std::unordered_map<std::string, int> m_lastRequestId;  // clientid -> requestID，一个 KV 服务器可以连接多个客户端

  int m_lastSnapShotRaftLogIndex;  // 记录最后一次生成快照时的 Raft 日志索引

 public:
  KvServer() = delete;  // 禁用默认构造函数

  // 构造函数，初始化服务器 ID、最大 Raft 状态以及节点信息文件名和端口
  KvServer(int me, int maxraftstate, std::string nodeInforFileName, short port);

  void StartKVServer();  // 启动键值服务器

  void DprintfKVDB();  // 打印键值数据库的内容

  // 执行追加操作
  void ExecuteAppendOpOnKVDB(Op op);

  // 执行获取操作
  void ExecuteGetOpOnKVDB(Op op, std::string *value, bool *exist);

  // 执行 Put 操作
  void ExecutePutOpOnKVDB(Op op);

  // 将 GetArgs 修改为 RPC 调用，因为服务器宕机对客户端无感知
  void Get(const raftKVRpcProctoc::GetArgs *args, raftKVRpcProctoc::GetReply* reply);  

  // 从 Raft 获取命令并执行
  void GetCommandFromRaft(ApplyMsg message);

  // 检查请求是否重复
  bool ifRequestDuplicate(std::string ClientId, int RequestId);

  // 通过 RPC 远程调用 Put 和 Append 操作
  void PutAppend(const raftKVRpcProctoc::PutAppendArgs *args, raftKVRpcProctoc::PutAppendReply *reply);

  // 一直等待 Raft 传来的 applyCh
  void ReadRaftApplyCommandLoop();

  // 从快照读取并安装
  void ReadSnapShotToInstall(std::string snapshot);

  // 向等待通道发送消息
  bool SendMessageToWaitChan(const Op &op, int raftIndex);

  // 检查是否需要生成快照，如果需要则发送快照命令
  void IfNeedToSendSnapShotCommand(int raftIndex, int proportion);

  // 从 Raft 获取快照数据
  void GetSnapShotFromRaft(ApplyMsg message);

  // 生成快照数据
  std::string MakeSnapShot();

 public:  // 为 RPC 提供的接口
  void PutAppend(google::protobuf::RpcController *controller, const ::raftKVRpcProctoc::PutAppendArgs *request,
                 ::raftKVRpcProctoc::PutAppendReply *response, ::google::protobuf::Closure *done) override;

  void Get(google::protobuf::RpcController *controller, const ::raftKVRpcProctoc::GetArgs *request,
           ::raftKVRpcProctoc::GetReply *response, ::google::protobuf::Closure *done) override;

  ///////////////// 序列化相关代码开始 ///////////////////////////////
 private:
  // 允许 Boost 序列化访问私有成员
  friend class boost::serialization::access;

  // 序列化函数，用于定义需要序列化和反序列化的字段
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &m_serializedKVData;  // 序列化 m_serializedKVData
    ar &m_lastRequestId;  // 序列化 m_lastRequestId
  }

  // 生成快照数据，将 SkipList 的数据序列化
  std::string getSnapshotData() {
    m_serializedKVData = m_skipList.dump_file();  // 将跳表的数据序列化为文件
    std::stringstream ss;  // 使用 stringstream 作为存储缓冲区
    boost::archive::text_oarchive oa(ss);  // 创建文本输出归档对象
    oa << *this;  // 序列化当前对象
    m_serializedKVData.clear();  // 清空序列化后的数据
    return ss.str();  // 返回序列化后的字符串
  }

  // 从字符串反序列化对象数据
  void parseFromString(const std::string &str) {
    std::stringstream ss(str);  // 使用字符串流来解析字符串
    boost::archive::text_iarchive ia(ss);  // 创建文本输入归档对象
    ia >> *this;  // 反序列化对象
    m_skipList.load_file(m_serializedKVData);  // 加载跳表的文件数据
    m_serializedKVData.clear();  // 清空序列化数据
  }

  ///////////////// 序列化相关代码结束 ///////////////////////////////
};

#endif  // SKIP_LIST_ON_RAFT_KVSERVER_H
