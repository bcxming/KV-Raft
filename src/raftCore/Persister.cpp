#include "Persister.h"
#include "util.h"

// todo: 这段代码涉及反复打开文件的操作，需要考虑如果文件出现问题时的处理方式

// 保存 Raft 状态和快照数据
void Persister::Save(const std::string raftstate, const std::string snapshot) {
  // 使用 std::lock_guard 保证线程安全，防止多个线程同时操作文件
  std::lock_guard<std::mutex> lg(m_mtx);

  // 清除之前保存的 Raft 状态和快照，准备写入新的数据
  clearRaftStateAndSnapshot();

  // 将新的 raftstate 和 snapshot 写入本地文件流
  m_raftStateOutStream << raftstate;
  m_snapshotOutStream << snapshot;
}

// 读取快照数据
std::string Persister::ReadSnapshot() {
  // 使用 std::lock_guard 保证线程安全
  std::lock_guard<std::mutex> lg(m_mtx);

  // 如果快照文件流已打开，先关闭以准备重新打开
  if (m_snapshotOutStream.is_open()) {
    m_snapshotOutStream.close();
  }

  // 使用 DEFER 宏确保函数结束时重新打开文件流
  DEFER {
    m_snapshotOutStream.open(m_snapshotFileName);  // 以追加模式重新打开
  };

  // 以读模式打开快照文件
  std::fstream ifs(m_snapshotFileName, std::ios_base::in);
  
  // 如果文件打开失败，则返回空字符串
  if (!ifs.good()) {
    return "";
  }

  // 从文件中读取快照数据
  std::string snapshot;
  ifs >> snapshot;

  // 关闭文件流
  ifs.close();
  
  return snapshot;
}

// 保存 Raft 状态数据
void Persister::SaveRaftState(const std::string &data) {
  // 使用 std::lock_guard 保证线程安全
  std::lock_guard<std::mutex> lg(m_mtx);

  // 清除现有的 Raft 状态数据
  clearRaftState();

  // 将新的 Raft 状态数据写入文件
  m_raftStateOutStream << data;

  // 更新 Raft 状态数据的大小
  m_raftStateSize += data.size();
}

// 返回当前 Raft 状态数据的大小
long long Persister::RaftStateSize() {
  // 使用 std::lock_guard 保证线程安全
  std::lock_guard<std::mutex> lg(m_mtx);

  return m_raftStateSize;
}

// 读取 Raft 状态数据
std::string Persister::ReadRaftState() {
  // 使用 std::lock_guard 保证线程安全
  std::lock_guard<std::mutex> lg(m_mtx);

  // 以读模式打开 Raft 状态文件
  std::fstream ifs(m_raftStateFileName, std::ios_base::in);

  // 如果文件无法打开，返回空字符串
  if (!ifs.good()) {
    return "";
  }

  // 从文件中读取 Raft 状态数据
  std::string snapshot;
  ifs >> snapshot;

  // 关闭文件流
  ifs.close();
  
  return snapshot;
}

// 构造函数，接收一个 `me` 参数，用于生成唯一的文件名
Persister::Persister(const int me)
    : m_raftStateFileName("raftstatePersist" + std::to_string(me) + ".txt"),
      m_snapshotFileName("snapshotPersist" + std::to_string(me) + ".txt"),
      m_raftStateSize(0) {
  
  // 检查文件是否能够成功打开并清空其内容
  bool fileOpenFlag = true;

  // 打开 Raft 状态文件（覆盖模式），如果打开成功，则关闭
  std::fstream file(m_raftStateFileName, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file.close();
  } else {
    fileOpenFlag = false;  // 如果文件无法打开，设置标志
  }

  // 打开快照文件（覆盖模式），如果打开成功，则关闭
  file = std::fstream(m_snapshotFileName, std::ios::out | std::ios::trunc);
  if (file.is_open()) {
    file.close();
  } else {
    fileOpenFlag = false;  // 如果文件无法打开，设置标志
  }

  // 如果有任何一个文件无法成功打开，打印错误信息
  if (!fileOpenFlag) {
    DPrintf("[func-Persister::Persister] file open error");
  }

  // 打开 Raft 状态和快照文件流，用于之后的读写操作
  m_raftStateOutStream.open(m_raftStateFileName);
  m_snapshotOutStream.open(m_snapshotFileName);
}

// 析构函数，关闭文件流
Persister::~Persister() {
  if (m_raftStateOutStream.is_open()) {
    m_raftStateOutStream.close();
  }
  if (m_snapshotOutStream.is_open()) {
    m_snapshotOutStream.close();
  }
}

// 清空 Raft 状态文件的内容
void Persister::clearRaftState() {
  m_raftStateSize = 0;  // 重置 Raft 状态数据大小

  // 关闭文件流
  if (m_raftStateOutStream.is_open()) {
    m_raftStateOutStream.close();
  }

  // 以覆盖模式重新打开 Raft 状态文件，清空内容
  m_raftStateOutStream.open(m_raftStateFileName, std::ios::out | std::ios::trunc);
}

// 清空快照文件的内容
void Persister::clearSnapshot() {
  // 关闭快照文件流
  if (m_snapshotOutStream.is_open()) {
    m_snapshotOutStream.close();
  }

  // 以覆盖模式重新打开快照文件，清空内容
  m_snapshotOutStream.open(m_snapshotFileName, std::ios::out | std::ios::trunc);
}

// 同时清空 Raft 状态文件和快照文件
void Persister::clearRaftStateAndSnapshot() {
  clearRaftState();   // 清空 Raft 状态
  clearSnapshot();    // 清空快照
}
