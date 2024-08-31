#ifndef APPLYMSG_H
#define APPLYMSG_H

#include <string>

//用于应用层向底层提交命令和快照，以便于底层的Raft协议进行处理和执行
class ApplyMsg {
 public:
  bool CommandValid;     // 指示命令是否有效
  std::string Command;   // 命令内容
  int CommandIndex;      // 命令索引
  bool SnapshotValid;    // 指示快照是否有效
  std::string Snapshot;  // 快照内容
  int SnapshotTerm;      // 快照的任期
  int SnapshotIndex;     // 快照索引

 public:
  // 构造函数，初始化所有成员变量
  ApplyMsg()
      : CommandValid(false),   // 初始化命令有效性为false
        Command(),             // 初始化命令为空字符串
        CommandIndex(-1),      // 初始化命令索引为-1
        SnapshotValid(false),  // 初始化快照有效性为false
        Snapshot(),            // 初始化快照为空字符串
        SnapshotTerm(-1),      // 初始化快照任期为-1
        SnapshotIndex(-1)      // 初始化快照索引为-1
        {};
};

#endif  // APPLYMSG_H
