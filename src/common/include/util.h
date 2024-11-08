
#ifndef UTIL_H
#define UTIL_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <condition_variable>  // pthread_condition_t
#include <functional>
#include <iostream>
#include <mutex>  // pthread_mutex_t
#include <queue>
#include <random>
#include <sstream>
#include <thread>
#include "config.h"

//这种技术通常用于在作用域结束时执行某些清理工作，比如释放资源、关闭文件等
// 定义一个模板类 DeferClass，模板参数 F 代表可调用对象的类型（如函数、lambda 等）
template <class F>
class DeferClass {
 public:
  // 构造函数1：接收右值引用的可调用对象，使用 std::forward 完美转发
  // 这样可以避免不必要的拷贝，并且支持传递临时对象（如 lambda 表达式）
  DeferClass(F&& f) : m_func(std::forward<F>(f)) {}

  // 构造函数2：接收左值引用的可调用对象并存储在成员变量中
  DeferClass(const F& f) : m_func(f) {}

  // 析构函数：在对象销毁时自动调用存储的可调用对象，确保作用域结束时执行一些操作
  ~DeferClass() { m_func(); }

  // 禁用复制构造函数，防止对象被拷贝
  DeferClass(const DeferClass& e) = delete;

  // 禁用赋值操作符，防止对象被赋值
  DeferClass& operator=(const DeferClass& e) = delete;

 private:
  // 成员变量，存储可调用对象
  F m_func;
};


// 将两个参数 a 和 b 连接成一个新的标识符
// 例如，_CONCAT(foo, bar) 将生成 foobar 这样的标识符
#define _CONCAT(a, b) a##b

// 使用当前行号生成一个唯一的 DeferClass 实例
// _MAKE_DEFER_ 会创建一个 DeferClass 对象，名称通过 _CONCAT 将 "defer_placeholder" 和当前行号拼接在一起，
// 并初始化为一个 lambda 表达式（使用 [&]() 捕获变量）
#define _MAKE_DEFER_(line) DeferClass _CONCAT(defer_placeholder, line) = [&]()

// 取消之前可能定义的 DEFER 宏，防止冲突
#undef DEFER

// 定义新的 DEFER 宏，它使用 _MAKE_DEFER_ 和当前代码行号（通过 __LINE__ 获取）生成唯一的 DeferClass 实例
// 这样在每个使用 DEFER 的地方，都会创建一个具有唯一名称的 DeferClass 对象，确保不会重名
#define DEFER _MAKE_DEFER_(__LINE__)

void DPrintf(const char* format, ...);

void myAssert(bool condition, std::string message = "Assertion failed!");

// 使用可变参数模板定义了一个格式化字符串函数，能够接受任意数量和类型的参数
template <typename... Args>
std::string format(const char* format_str, Args... args) {
  std::stringstream ss;  // 创建一个字符串流，用于拼接参数
  // 利用逗号表达式展开参数包，将每个参数都插入到字符串流 ss 中
  // 通过构造一个临时数组 _，每个元素执行 `ss << args`，然后忽略数组内容
  int _[] = {((ss << args), 0)...};
  
  // 通过 (void)_; 来避免未使用变量的警告
  (void)_;

  // 返回拼接后的字符串
  return ss.str();
}


std::chrono::_V2::system_clock::time_point now();

std::chrono::milliseconds getRandomizedElectionTimeout();
void sleepNMilliseconds(int N);

// ////////////////////////异步写日志的日志队列
// read is blocking!!! LIKE  go chan
template <typename T>
class LockQueue {
 public:
  // 多个worker线程都会写日志queue
  void Push(const T& data) {
    std::lock_guard<std::mutex> lock(m_mutex);  //使用lock_gurad，即RAII的思想保证锁正确释放
    m_queue.push(data);
    m_condvariable.notify_one();
  }

  // 一个线程读日志queue，写日志文件
  T Pop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    while (m_queue.empty()) {
      // 日志队列为空，线程进入wait状态
      m_condvariable.wait(lock);  //这里用unique_lock是因为lock_guard不支持解锁，而unique_lock支持
    }
    T data = m_queue.front();
    m_queue.pop();
    return data;
  }

  bool timeOutPop(int timeout, T* ResData)  // 添加一个超时时间参数，默认为 50 毫秒
  {
    std::unique_lock<std::mutex> lock(m_mutex);

    // 获取当前时间点，并计算出超时时刻
    auto now = std::chrono::system_clock::now();
    auto timeout_time = now + std::chrono::milliseconds(timeout);

    // 在超时之前，不断检查队列是否为空
    while (m_queue.empty()) {
      // 如果已经超时了，就返回一个空对象
      if (m_condvariable.wait_until(lock, timeout_time) == std::cv_status::timeout) {
        return false;
      } else {
        continue;
      }
    }

    T data = m_queue.front();
    m_queue.pop();
    *ResData = data;
    return true;
  }

 private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_condvariable;
};

// 这个Op是kv传递给raft的command
class Op {
 public:
  // 字段名必须以大写字母开头，否则会影响RPC通信
  std::string Operation;  // "Get" "Put" "Append"
  std::string Key;// 键值
  std::string Value;// 值
  std::string ClientId;  //客户端号码
  int RequestId;         //客户端号码请求的Request的序列号，为了保证线性一致性

 public:
  // todo
  //为了协调raftRPC中的command只设置成了string,这个的限制就是正常字符中不能包含|
  //当然后期可以换成更高级的序列化方法，比如protobuf
  std::string asString() const {
    std::stringstream ss;
    boost::archive::text_oarchive oa(ss);

    // 将Op对象写入存档
    oa << *this;
    // close archive

    return ss.str();
  }

  bool parseFromString(std::string str) {
    std::stringstream iss(str);
    boost::archive::text_iarchive ia(iss);
    ia >> *this;
    return true;  // todo: 如何处理解析失败，需要查看boost库
  }

 public:
 // 重载流输出操作符，以自定义格式打印Op对象
  friend std::ostream& operator<<(std::ostream& os, const Op& obj) {
    os << "[MyClass:Operation{" + obj.Operation + "},Key{" + obj.Key + "},Value{" + obj.Value + "},ClientId{" +
              obj.ClientId + "},RequestId{" + std::to_string(obj.RequestId) + "}";  // 在这里实现自定义的输出格式
    return os;
  }

 private:
  friend class boost::serialization::access;
    // 序列化Op对象
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& Operation;
    ar& Key;
    ar& Value;
    ar& ClientId;
    ar& RequestId;
  }
};

///////////////////////////////////////////////kvserver reply err to clerk

const std::string OK = "OK";
const std::string ErrNoKey = "ErrNoKey";
const std::string ErrWrongLeader = "ErrWrongLeader";

////////////////////////////////////获取可用端口

bool isReleasePort(unsigned short usPort);

bool getReleasePort(short& port);


#endif  //  UTIL_H