# 基于Raft实现的分布式存储

在当今大规模分布式系统环境中，构建一个**可靠且高可用**的分布式数据存储系统至关重要。

传统的集中式数据库在处理**海量数据和高并发请求**时，容易出现单点故障和性能瓶颈。

为此，本项目旨在基于Raft一致性算法开发一个分布式键值存储数据库，以实现数据的一致性、可用性和分区容错能力，从而解决这些挑战。

## 项目介绍

### **分布式存储**

意义：为了提高大规模数据的读写性能：

- 更好的机器：硬件的性能提升是有上线的
- 更多的机器：多台机器构成集群对外提供服务

优点：数据备份和负载均衡

缺点：节点间数据的不一致性；如何保证分布式系统的秩序

分类：横向分布式(同一模块，为了提高系统吞吐量而采用的多节点);  纵向分布式(基于职责而进行模块划分并通过 RPC串联整体)

**什么叫CAP**

一致性：强一致和弱一致

可用性：不发生错误，也不能有过长的等待时间

分区容错性：强调分区系统还能正常运行

### **Raft**

Raft算法是一种分布式算法，旨在解决分布式系统中的一致性问题

Raft算法将系统中的所有节点分为三类角色：领导者（`leader`）、跟随者（`follower`）和候选人（`candidate`）。其选举机制确保系统中的一个节点被选为领导者（`leader`），领导者负责处理客户端的请求，并将更新复制到其他节点。

Raft算法的基本原理包括以下几个关键步骤：

- 领导者选举（Leader Election）：在系统启动时或者当前领导者失效时，节点会发起选举过程。节点会在一个随机的超时时间内等待收到来自其他节点的心跳消息。如果在超时时间内没有收到心跳消息，节点就会成为候选人并发起选举。候选人向其他节点发送投票请求，并在得到大多数节点的投票后成为新的领导者。

- 日志复制（Log Replication）：一旦领导者选举完成，新的领导者就会接收客户端的请求，并将更新的日志条目复制到其他节点。当大多数节点都成功复制了这些日志条目时，更新被认为是提交的，并且可以应用到节点的状态机中。

- 安全性（Safety）：Raft算法通过确保在选举中只有一个领导者（单一领导者）、大多数节点的一致性以及只有领导者可以处理客户端请求等方式保证分布式系统的安全性。

### RPC

RPC（Remote Procedure Call，远程过程调用）是一种通信协议，允许程序调用位于另一台计算机上的函数或过程，就像调用本地函数一样。RPC的主要目的是使得分布式计算更加简单和透明。

- **透明性**：调用远程服务的过程与本地函数调用相似，屏蔽了底层的网络通信细节。

- **序列化和反序列化**：数据在网络上传输时需要进行序列化（将数据结构转换为字节流）和反序列化（将字节流还原为数据结构）。

- **通信协议**：RPC依赖底层网络协议（如HTTP、TCP）来实现远程通信。

- **错误处理**：因为网络通信的不可预见性，RPC需要处理各种可能的错误，如超时、连接失败等。

### 跳表

**跳表（Skip List）**是一种用于实现有序集合的数据结构，它在链表的基础上增加了多级索引，允许快速地进行查找、插入和删除操作。跳表的时间复杂度为 O(log n)，与平衡树类似，但实现起来更简单且更易于维护。

- **多级索引**：跳表在链表的基础上增加了多层索引，每层是对下一层的抽样，这样可以通过“跳跃”的方式加速查找。

- **平衡性**：通过随机算法来决定每个元素所处的索引层数，确保平均性能。

- **操作复杂度**：查找、插入、删除操作的时间复杂度都是 O(log n)，而空间复杂度为 O(n)。

和其他数据结构的比较

**跳表 vs. 平衡树（如AVL树、红黑树）**:

- **相同点**：二者都支持快速的查找、插入和删除操作，平均时间复杂度为 O(log n)。
- **不同点**：跳表的实现简单；而平衡树的插入和删除需要重新平衡，操作较复杂。

**跳表 vs. 哈希表**:

- **相同点**：哈希表和跳表都能实现快速查找。
- **不同点**：哈希表的查找、插入、删除的平均时间复杂度为 O(1)，但不支持有序操作（如范围查询）；跳表支持有序操作和范围查询，而哈希表不支持。

**跳表 vs. 链表**:

- **相同点**：两者都是基于指针的结构，操作简单。
- **不同点**：链表的查找时间复杂度为 O(n)，而跳表通过多级索引将查找时间复杂度降至 O(log n)。

**跳表 vs. 二分查找**（适用于数组）:

- **相同点**：都可以在有序集合中快速查找，时间复杂度为 O(log n)。
- **不同点**：二分查找只能在静态数组中使用，插入和删除操作时间复杂度为 O(n)，不如跳表灵活。

## 技术介绍

### RPC

## 目录结构

```txt

```

## 环境介绍

## 快速开始

## 总结&鸣谢

