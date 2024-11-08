#include "util.h"
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iomanip>
//用于实现类似于标准库中 assert 宏的功能，即在条件不满足时输出错误信息并终止程序执行。
void myAssert(bool condition, std::string message) {
  if (!condition) {
    std::cerr << "Error: " << message << std::endl;
    std::exit(EXIT_FAILURE);
  }
}
//用于获取当前系统时间
std::chrono::_V2::system_clock::time_point now() { return std::chrono::high_resolution_clock::now(); }

//用于生成一个随机的选举超时时间。
std::chrono::milliseconds getRandomizedElectionTimeout() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> dist(minRandomizedElectionTime, maxRandomizedElectionTime);

  return std::chrono::milliseconds(dist(rng));
}

//用于让当前线程休眠指定的毫秒数。
void sleepNMilliseconds(int N) { std::this_thread::sleep_for(std::chrono::milliseconds(N)); };

//用于获取一个可用的端口号，并将该端口号存储在参数 port 中
bool getReleasePort(short &port) {
  short num = 0;
  while (!isReleasePort(port) && num < 30) {
    ++port;
    ++num;
  }
  if (num >= 30) {
    port = -1;
    return false;
  }
  return true;
}

//用于检测指定的端口是否处于可用状态（即未被占用）。
bool isReleasePort(unsigned short usPort) {
  int s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(usPort);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  int ret = ::bind(s, (sockaddr *)&addr, sizeof(addr));
  if (ret != 0) {
    close(s);
    return false;
  }
  close(s);
  return true;
}

//用于在调试模式下打印日志信息。函数采用与 printf 类似的可变参数列表，允许将格式化的字符串和参数作为输入
void DPrintf(const char *format, ...) {
  if (Debug) {
    // 获取当前的日期，然后取日志信息，写入相应的日志文件当中 a+
    time_t now = time(nullptr);
    tm *nowtm = localtime(&now);
    va_list args;
    va_start(args, format);
    std::printf("[%d-%d-%d-%d-%d-%d] ", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday, nowtm->tm_hour,
                nowtm->tm_min, nowtm->tm_sec);
    std::vprintf(format, args);
    std::printf("\n");
    va_end(args);
  }
}
