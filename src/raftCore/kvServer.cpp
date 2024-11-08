#include "kvServer.h"

#include <rpcprovider.h>

#include "mprpcconfig.h"

void KvServer::DprintfKVDB() {
  if (!Debug) {
    return;
  }
  std::lock_guard<std::mutex> lg(m_mtx);
  DEFER {
    m_skipList.display_list();
  };
}

void KvServer::ExecuteAppendOpOnKVDB(Op op) {
  m_mtx.lock();

  m_skipList.insert_set_element(op.Key, op.Value);
  m_lastRequestId[op.ClientId] = op.RequestId;
  m_mtx.unlock();
  DprintfKVDB();
}

void KvServer::ExecuteGetOpOnKVDB(Op op, std::string *value, bool *exist) {
  m_mtx.lock();
  *value = "";
  *exist = false;
  if (m_skipList.search_element(op.Key, *value)) {
    *exist = true;
  }
  m_lastRequestId[op.ClientId] = op.RequestId;
  m_mtx.unlock();
  if (*exist) {
  } else {
  }
  DprintfKVDB();
}

void KvServer::ExecutePutOpOnKVDB(Op op) {
  m_mtx.lock();
  m_skipList.insert_set_element(op.Key, op.Value);
  m_lastRequestId[op.ClientId] = op.RequestId;
  m_mtx.unlock();
  DprintfKVDB();
}

// 处理来自 clerk（客户端）的 Get RPC 请求
void KvServer::Get(const raftKVRpcProctoc::GetArgs *args, raftKVRpcProctoc::GetReply *reply) {
  
  // 创建一个操作对象 op，用于记录 Get 操作的相关信息
  Op op;
  op.Operation = "Get";              // 设置操作类型为 "Get"
  op.Key = args->key();              // 从 RPC 请求中获取需要查询的键
  op.Value = "";                     // 初始化操作的值为空字符串
  op.ClientId = args->clientid();    // 记录客户端 ID
  op.RequestId = args->requestid();  // 记录请求 ID，方便后续判断是否是重复请求

  // 发起 Raft 操作并获取操作的 Raft 日志索引和是否为当前领导者
  int raftIndex = -1;  // 用于存储 Raft 日志索引
  int _ = -1;          // 用于存储不关心的返回值
  bool isLeader = false;                             // 用于判断当前节点是否是领导者
  m_raftNode->Start(op, &raftIndex, &_, &isLeader);  // 调用 Raft 节点的 Start 函数开始处理操作

  // 如果当前节点不是 Raft 集群中的领导者，则返回错误信息给客户端，表示请求被发送到了非领导者节点
  if (!isLeader) {
    reply->set_err(ErrWrongLeader);  // 设置错误类型为“不是领导者”
    return;
  }

  // 上锁以保护共享资源 waitApplyCh（一个等待应用操作的通道映射）
  m_mtx.lock();

  // 如果 waitApplyCh（等待应用操作的队列）中没有与当前 raftIndex 对应的通道，则创建一个新的通道并添加到队列中
  if (waitApplyCh.find(raftIndex) == waitApplyCh.end()) {
    waitApplyCh.insert(std::make_pair(raftIndex, new LockQueue<Op>()));  // 创建新通道
  }
  // 获取与当前 Raft 日志索引关联的通道
  auto chForRaftIndex = waitApplyCh[raftIndex];

  // 解锁以允许其他线程继续执行
  m_mtx.unlock();

  // 检查超时情况并处理，如果 Raft 操作在超时时间内未完成，则进行超时处理
  Op raftCommitOp;
  if (!chForRaftIndex->timeOutPop(CONSENSUS_TIMEOUT, &raftCommitOp)) {
    // 超时处理：再次检查是否仍然是领导者
    int _ = -1;
    bool isLeader = false;
    m_raftNode->GetState(&_, &isLeader);

    // 如果请求是重复的（通过检查客户端 ID 和请求 ID），并且当前节点仍然是领导者，则在数据库中执行 Get 操作
    if (ifRequestDuplicate(op.ClientId, op.RequestId) && isLeader) {
      std::string value;
      bool exist = false;
      ExecuteGetOpOnKVDB(op, &value, &exist);  // 在数据库中执行 Get 操作

      // 如果键存在，返回正确的值；否则返回 ErrNoKey 错误
      if (exist) {
        reply->set_err(OK);
        reply->set_value(value);
      } else {
        reply->set_err(ErrNoKey);
        reply->set_value("");
      }
    } else {
      // 如果请求不是重复的或当前节点不是领导者，则返回错误信息
      reply->set_err(ErrWrongLeader);
    }
  } else {
    // Raft 操作成功提交后，在数据库中执行 Get 操作
    if (raftCommitOp.ClientId == op.ClientId && raftCommitOp.RequestId == op.RequestId) {
      std::string value;
      bool exist = false;
      ExecuteGetOpOnKVDB(op, &value, &exist);  // 执行数据库操作以获取值

      // 如果键存在，返回正确的值；否则返回 ErrNoKey 错误
      if (exist) {
        reply->set_err(OK);
        reply->set_value(value);
      } else {
        reply->set_err(ErrNoKey);
        reply->set_value("");
      }
    } else {
      // 如果 Raft 操作提交的客户端 ID 和请求 ID 不匹配，则返回错误信息
      reply->set_err(ErrWrongLeader);
    }
  }

  // 重新上锁以安全地删除对应 Raft 日志索引的等待通道
  m_mtx.lock();
  auto tmp = waitApplyCh[raftIndex];  // 获取对应的通道
  waitApplyCh.erase(raftIndex);       // 从映射表中移除该通道
  delete tmp;                         // 释放通道的内存
  m_mtx.unlock();
}


// 从 Raft 节点获取命令并在 KVServer 中执行对应的操作
void KvServer::GetCommandFromRaft(ApplyMsg message) {
  // 解析从 Raft 节点传递来的消息中的命令，将其转换为 Op 对象
  Op op;
  op.parseFromString(message.Command);

  // 打印调试信息，显示收到的 Raft 命令的详细信息
  DPrintf(
      "[KvServer::GetCommandFromRaft-kvserver{%d}] , Got Command --> Index:{%d} , ClientId {%s}, RequestId {%d}, "
      "Opreation {%s}, Key :{%s}, Value :{%s}",
      m_me, message.CommandIndex, &op.ClientId, op.RequestId, &op.Operation, &op.Key, &op.Value);

  // 如果当前命令的索引小于等于上次制作快照时的 Raft 日志索引，表示该命令已经被处理过，直接返回
  if (message.CommandIndex <= m_lastSnapShotRaftLogIndex) {
    return;
  }

  // 状态机处理逻辑（KVServer 解决重复请求的问题）
  // 如果请求是重复的，则不再执行命令
  if (!ifRequestDuplicate(op.ClientId, op.RequestId)) {
    // 根据命令的操作类型执行相应的操作
    if (op.Operation == "Put") {
      ExecutePutOpOnKVDB(op);  // 执行 Put 操作，将键值对放入 KV 数据库中
    }
    if (op.Operation == "Append") {
      ExecuteAppendOpOnKVDB(op);  // 执行 Append 操作，将值追加到现有键的值后面
    }
    // 注意：kv.lastRequestId[op.ClientId] 在 ExecutePutOpOnKVDB 和 ExecuteAppendOpOnKVDB 函数中会被更新
  }

  // 如果达到指定的 Raft 日志大小限制，则检查是否需要制作快照
  if (m_maxRaftState != -1) {
    // 如果 Raft 日志大小超过比例（比如 90%），则触发快照操作
    IfNeedToSendSnapShotCommand(message.CommandIndex, 9);
  }

  // 将操作结果发送到与客户端 ID 关联的等待通道（客户端可以接收操作结果）
  SendMessageToWaitChan(op, message.CommandIndex);
}


bool KvServer::ifRequestDuplicate(std::string ClientId, int RequestId) {
  std::lock_guard<std::mutex> lg(m_mtx);
  if (m_lastRequestId.find(ClientId) == m_lastRequestId.end()) {
    return false;
    // todo :不存在这个client就创建
  }
  return RequestId <= m_lastRequestId[ClientId];
}

// PutAppend函数是处理客户端的Put和Append请求
// Get与Put/Append操作的主要区别在于：Get操作即使是重复请求也会被重新执行，
// 而Put/Append操作会通过冪等性判断，避免重复执行。

void KvServer::PutAppend(const raftKVRpcProctoc::PutAppendArgs *args, raftKVRpcProctoc::PutAppendReply *reply) {
  // 构造一个操作对象 op，用于记录 Put/Append 请求的相关信息
  Op op;
  op.Operation = args->op();         // 设置操作类型（Put 或 Append）
  op.Key = args->key();              // 从请求中获取键
  op.Value = args->value();          // 从请求中获取值
  op.ClientId = args->clientid();    // 从请求中获取客户端ID
  op.RequestId = args->requestid();  // 从请求中获取请求ID

  int raftIndex = -1;  // 用于存储 Raft 日志索引
  int _ = -1;          // 无关的返回值
  bool isleader = false;  // 标记当前节点是否为 Raft 集群的领导者

  // 启动 Raft 操作，将请求传递给 Raft 日志
  m_raftNode->Start(op, &raftIndex, &_, &isleader);

  // 如果当前节点不是领导者，返回错误信息给客户端
  if (!isleader) {
    DPrintf(
        "[func -KvServer::PutAppend -kvserver{%d}]From Client %s (Request %d) To Server %d, key %s, raftIndex %d , but "
        "not leader",
        m_me, &args->clientid(), args->requestid(), m_me, &op.Key, raftIndex);

    reply->set_err(ErrWrongLeader);  // 返回“不是领导者”错误
    return;
  }

  // 打印日志，表明当前服务器是领导者并已处理客户端请求
  DPrintf(
      "[func -KvServer::PutAppend -kvserver{%d}]From Client %s (Request %d) To Server %d, key %s, raftIndex %d , is "
      "leader ",
      m_me, &args->clientid(), args->requestid(), m_me, &op.Key, raftIndex);

  // 使用互斥锁保护 waitApplyCh 映射表的并发操作
  m_mtx.lock();
  // 如果 Raft 日志索引对应的通道不存在，创建一个新的通道并插入映射表中
  if (waitApplyCh.find(raftIndex) == waitApplyCh.end()) {
    waitApplyCh.insert(std::make_pair(raftIndex, new LockQueue<Op>()));
  }
  // 获取与当前 Raft 日志索引关联的通道
  auto chForRaftIndex = waitApplyCh[raftIndex];
  m_mtx.unlock();  // 解锁，避免在等待过程中持有锁

  // 定义 Raft 提交的操作对象
  Op raftCommitOp;

  // 等待 Raft 日志应用，若超时未收到响应则进行处理
  if (!chForRaftIndex->timeOutPop(CONSENSUS_TIMEOUT, &raftCommitOp)) {
    // 超时处理，打印超时日志
    DPrintf(
        "[func -KvServer::PutAppend -kvserver{%d}]TIMEOUT PUTAPPEND !!!! Server %d , get Command <-- Index:%d , "
        "ClientId %s, RequestId %s, Opreation %s Key :%s, Value :%s",
        m_me, m_me, raftIndex, &op.ClientId, op.RequestId, &op.Operation, &op.Key, &op.Value);

    // 如果请求是重复的（通过客户端ID和请求ID判断），返回 OK
    if (ifRequestDuplicate(op.ClientId, op.RequestId)) {
      reply->set_err(OK);  // 返回成功，尽管超时，因为这是一个重复请求
    } else {
      // 否则返回错误，告知客户端不是领导者，客户端会重试
      reply->set_err(ErrWrongLeader);
    }
  } else {
    // 如果 Raft 成功提交了操作，验证提交的操作是否与当前操作一致
    DPrintf(
        "[func -KvServer::PutAppend -kvserver{%d}]WaitChanGetRaftApplyMessage<--Server %d , get Command <-- Index:%d , "
        "ClientId %s, RequestId %d, Opreation %s, Key :%s, Value :%s",
        m_me, m_me, raftIndex, &op.ClientId, op.RequestId, &op.Operation, &op.Key, &op.Value);
    
    // 如果提交的操作的客户端ID和请求ID与当前操作一致，说明是正确的操作
    if (raftCommitOp.ClientId == op.ClientId && raftCommitOp.RequestId == op.RequestId) {
      reply->set_err(OK);  // 返回成功
    } else {
      // 否则可能是由于领导者变化导致的操作不一致，返回错误信息
      reply->set_err(ErrWrongLeader);
    }
  }

  // 清理等待通道
  m_mtx.lock();
  auto tmp = waitApplyCh[raftIndex];  // 获取对应的等待通道
  waitApplyCh.erase(raftIndex);       // 从映射表中移除通道
  delete tmp;                         // 释放通道的内存
  m_mtx.unlock();
}


// 该函数负责持续监听 Raft 节点的日志应用命令，并对每个命令进行处理。
// 它运行在一个循环中，不断从 Raft 节点中接收日志应用消息，并根据消息的类型执行相应的操作。
void KvServer::ReadRaftApplyCommandLoop() {
  // 无限循环，持续监听 Raft 应用的日志命令
  while (true) {
    // 从 applyChan（Raft 应用的消息通道）中阻塞弹出消息
    // 由于 applyChan 自带锁机制，这里不需要额外使用锁保护
    auto message = applyChan->Pop();  // 阻塞等待 Raft 应用的消息

    // 打印调试信息，表示当前服务器收到了来自 Raft 的消息
    DPrintf(
        "---------------tmp-------------[func-KvServer::ReadRaftApplyCommandLoop()-kvserver{%d}] 收到了来自raft的消息",
        m_me);

    // 如果消息中的 CommandValid 为 true，表示该消息是一个有效的命令
    // 调用 GetCommandFromRaft 函数处理该命令
    if (message.CommandValid) {
      GetCommandFromRaft(message);
    }

    // 如果消息中的 SnapshotValid 为 true，表示该消息是一个快照数据
    // 调用 GetSnapShotFromRaft 函数处理快照
    if (message.SnapshotValid) {
      GetSnapShotFromRaft(message);
    }
  }
}


// 该函数负责将 Raft 层传递的快照数据恢复到 KvServer 中，以便重建 kvServer 的状态。
// Raft 层与 persist 层交互，kvServer 也会与 persist 层交互，
// 例如在服务器启动时恢复 kvDB 的状态，或通过快照机制保持数据的一致性。
// 快照中包含 kvServer 需要保存的持久化状态，包括客户端请求ID和键值数据库内容。

void KvServer::ReadSnapShotToInstall(std::string snapshot) {
  // 如果传递过来的快照是空的，表示没有任何状态需要恢复，可能是系统初次启动
  if (snapshot.empty()) {
    // 无状态启动（bootstrap without any state）
    return;
  }

  // 调用 parseFromString 函数，将传入的快照数据进行解析，恢复 kvServer 的状态
  // 快照中包含客户端的最后请求ID（persist_lastRequestId）和真正保存的 kvDB 数据（persist_kvdb）
  parseFromString(snapshot);
}


// 该函数将操作消息发送到等待通道，供后续处理。
// 它通过 Raft 日志索引（raftIndex）找到对应的等待通道，将操作（Op）传递进去。
// 如果成功找到对应的通道，操作将被推送到通道中，并返回 true，表示成功发送。
// 否则，返回 false，表示未找到对应的等待通道。

bool KvServer::SendMessageToWaitChan(const Op &op, int raftIndex) {
  // 使用 std::lock_guard 自动管理互斥锁，确保 m_mtx 在此函数作用范围内被锁定
  std::lock_guard<std::mutex> lg(m_mtx);

  // 打印调试信息，显示当前服务器将 Raft 操作消息发送到等待通道的细节
  DPrintf(
      "[RaftApplyMessageSendToWaitChan--> raftserver{%d}] , Send Command --> Index:{%d} , ClientId {%d}, RequestId "
      "{%d}, Opreation {%v}, Key :{%v}, Value :{%v}",
      m_me, raftIndex, &op.ClientId, op.RequestId, &op.Operation, &op.Key, &op.Value);

  // 检查等待通道（waitApplyCh）中是否存在与 Raft 日志索引（raftIndex）对应的通道
  // 如果找不到该通道，直接返回 false
  if (waitApplyCh.find(raftIndex) == waitApplyCh.end()) {
    return false;
  }

  // 如果找到通道，将操作（Op）推送到对应的通道中
  waitApplyCh[raftIndex]->Push(op);

  // 再次打印调试信息，表明操作已成功发送到通道
  DPrintf(
      "[RaftApplyMessageSendToWaitChan--> raftserver{%d}] , Send Command --> Index:{%d} , ClientId {%d}, RequestId "
      "{%d}, Opreation {%v}, Key :{%v}, Value :{%v}",
      m_me, raftIndex, &op.ClientId, op.RequestId, &op.Operation, &op.Key, &op.Value);

  // 返回 true，表示操作成功发送到等待通道
  return true;
}


// 该函数用于检查是否需要发送生成快照的命令，以防止 Raft 日志大小过大。
// 当 Raft 日志大小超过设定的阈值时，kvServer 会生成一个快照并通知 Raft 层进行快照保存。
void KvServer::IfNeedToSendSnapShotCommand(int raftIndex, int proportion) {
  // 检查 Raft 当前的日志状态大小是否超过最大允许的日志大小比例（m_maxRaftState 的十分之一）
  // 如果日志大小超过这个阈值，说明需要生成快照以防止日志过大
  if (m_raftNode->GetRaftStateSize() > m_maxRaftState / 10.0) {
    // 调用 MakeSnapShot 函数生成 kvServer 的快照
    auto snapshot = MakeSnapShot();
    
    // 通知 Raft 节点，生成快照并保存当前 Raft 日志索引位置的快照数据
    m_raftNode->Snapshot(raftIndex, snapshot);
  }
}


// 该函数处理从 Raft 接收到的快照数据，首先判断快照是否可以被应用，
// 如果可以，则将快照应用到 kvServer 的状态中，并更新最后应用的 Raft 日志索引。
void KvServer::GetSnapShotFromRaft(ApplyMsg message) {
  // 使用 std::lock_guard 自动管理互斥锁，确保对共享资源的访问是线程安全的
  std::lock_guard<std::mutex> lg(m_mtx);

  // 调用 Raft 节点的 CondInstallSnapshot 函数，检查是否可以安装快照
  // 根据快照的 Term 和 Index 判断该快照是否可以被安装
  if (m_raftNode->CondInstallSnapshot(message.SnapshotTerm, message.SnapshotIndex, message.Snapshot)) {
    // 如果可以安装，则读取快照数据并将其应用到 kvServer 的状态
    ReadSnapShotToInstall(message.Snapshot);
    
    // 更新最后一次快照应用的 Raft 日志索引，记录当前的日志状态
    m_lastSnapShotRaftLogIndex = message.SnapshotIndex;
  }
}


// 该函数生成当前 kvServer 的快照，包含键值数据库和客户端请求的状态，
// 返回一个序列化后的字符串作为快照数据。

std::string KvServer::MakeSnapShot() {
  // 使用 std::lock_guard 自动管理互斥锁，确保对共享资源的访问是线程安全的
  std::lock_guard<std::mutex> lg(m_mtx);

  // 调用 getSnapshotData 函数生成快照数据（将 kvServer 的状态序列化）
  std::string snapshotData = getSnapshotData();
  
  // 返回生成的快照数据
  return snapshotData;
}


void KvServer::PutAppend(google::protobuf::RpcController *controller, const ::raftKVRpcProctoc::PutAppendArgs *request,
                         ::raftKVRpcProctoc::PutAppendReply *response, ::google::protobuf::Closure *done) {
  KvServer::PutAppend(request, response);
  done->Run();
}

void KvServer::Get(google::protobuf::RpcController *controller, const ::raftKVRpcProctoc::GetArgs *request,
                   ::raftKVRpcProctoc::GetReply *response, ::google::protobuf::Closure *done) {
  KvServer::Get(request, response);
  done->Run();
}
// KvServer 构造函数，用于初始化键值服务器与 Raft 节点，设置持久化和 RPC 通信功能。
// 该构造函数接受参数：当前节点ID me，最大 Raft 状态大小 maxraftstate，
// 节点信息文件名 nodeInforFileName，以及端口号 port。

KvServer::KvServer(int me, int maxraftstate, std::string nodeInforFileName, short port) : m_skipList(6) {
  // 创建 Persister 对象，用于持久化当前节点的状态。
  std::shared_ptr<Persister> persister = std::make_shared<Persister>(me);

  // 初始化当前服务器节点的ID 和 最大 Raft 日志状态大小。
  m_me = me;
  m_maxRaftState = maxraftstate;

  // 创建用于 Raft 和 KvServer 之间通信的消息队列。
  applyChan = std::make_shared<LockQueue<ApplyMsg> >();

  // 初始化 Raft 节点，m_raftNode 是 Raft 协议的核心节点对象。
  m_raftNode = std::make_shared<Raft>();

  // 开启一个线程用于启动 RPC 服务，接受来自 Clerk 和其他 Raft 节点的远程调用。
  // KvServer 和 Raft 节点都需要支持 RPC，因此有两个 NotifyService 注册。
  std::thread t([this, port]() -> void {
    RpcProvider provider;  // 创建一个 RPC 网络服务对象。
    provider.NotifyService(this);  // 注册 KvServer 的 RPC 服务。
    provider.NotifyService(this->m_raftNode.get());  // 注册 Raft 节点的 RPC 服务，获取原始指针。
    
    // 启动 RPC 服务并进入阻塞状态，等待远程调用请求。
    provider.Run(m_me, port);
  });
  t.detach();  // 线程分离，独立运行。

  // 等待所有 Raft 节点启动并开启 RPC 服务，为保证所有节点能相互连接，睡眠6秒。
  std::cout << "raftServer node:" << m_me << " start to sleep to wait all ohter raftnode start!!!!" << std::endl;
  sleep(6);  // 睡眠一段时间，确保其他 Raft 节点启动完成。
  std::cout << "raftServer node:" << m_me << " wake up!!!! start to connect other raftnode" << std::endl;

  // 读取节点信息文件，获取所有其他 Raft 节点的 IP 和端口，并建立连接。
  MprpcConfig config;
  config.LoadConfigFile(nodeInforFileName.c_str());
  std::vector<std::pair<std::string, short>> ipPortVt;  // 存储所有节点的 IP 和端口。

  // 遍历所有节点并加载其 IP 和端口信息，直到读取不到更多节点信息为止。
  for (int i = 0; i < INT_MAX - 1; ++i) {
    std::string node = "node" + std::to_string(i);

    // 读取节点的 IP 地址和端口号。
    std::string nodeIp = config.Load(node + "ip");
    std::string nodePortStr = config.Load(node + "port");
    if (nodeIp.empty()) {
      break;  // 如果没有读取到 IP，表示所有节点信息读取完成。
    }
    ipPortVt.emplace_back(nodeIp, atoi(nodePortStr.c_str()));  // 将节点 IP 和端口存储到 ipPortVt 向量中。
  }

  // 连接到其他 Raft 节点，构建与其他节点的 RPC 通信。
  std::vector<std::shared_ptr<RaftRpcUtil>> servers;
  for (int i = 0; i < ipPortVt.size(); ++i) {
    if (i == m_me) {  // 跳过自身节点，不需要连接自己。
      servers.push_back(nullptr);
      continue;
    }
    // 连接到其他节点的 IP 和端口。
    std::string otherNodeIp = ipPortVt[i].first;
    short otherNodePort = ipPortVt[i].second;
    auto *rpc = new RaftRpcUtil(otherNodeIp, otherNodePort);  // 创建一个 RaftRpcUtil 对象，用于连接其他节点。
    servers.push_back(std::shared_ptr<RaftRpcUtil>(rpc));  // 使用智能指针管理。

    std::cout << "node" << m_me << " 连接node" << i << "success!" << std::endl;  // 打印连接成功信息。
  }

  // 等待所有节点连接成功，再启动 Raft 协议。
  sleep(ipPortVt.size() - me);  // 睡眠一段时间，确保节点之间的连接成功。

  // 初始化 Raft 节点，传入所有服务器的 RPC 连接、当前节点 ID、持久化对象和消息队列。
  m_raftNode->init(servers, m_me, persister, applyChan);

  // 初始化 KVServer 的状态。
  m_skipList;  // 初始化跳表，用于存储键值对。
  waitApplyCh;  // 初始化等待应用的操作队列。
  m_lastRequestId;  // 初始化客户端的最后请求 ID。
  m_lastSnapShotRaftLogIndex = 0;  // 初始化最后的快照日志索引。

  // 如果持久化中有快照，读取并恢复状态。
  auto snapshot = persister->ReadSnapshot();
  if (!snapshot.empty()) {
    ReadSnapShotToInstall(snapshot);  // 从快照恢复状态。
  }

  // 启动一个线程来监听 Raft 日志的应用命令。
  std::thread t2(&KvServer::ReadRaftApplyCommandLoop, this);
  t2.join();  // 由于 ReadRaftApplyCommandLoop 是一个无限循环，join 保证该线程一直运行。
}
