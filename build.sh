#!/bin/bash

# 切换到 cmake-build-debug 目录
cd cmake-build-debug || { echo "Failed to change directory to cmake-build-debug"; exit 1; }

# 运行 cmake
echo "Running cmake..."
cmake .. || { echo "CMake failed"; exit 1; }

# 运行 make 编译
echo "Building the project..."
make || { echo "Make failed"; exit 1; }

# 返回上一级目录
cd .. || { echo "Failed to change directory"; exit 1; }

# 切换到 bin 目录
cd bin || { echo "Failed to change directory to bin"; exit 1; }

# 运行 raftCoreRun 程序
echo "Running raftCoreRun with -n 3 -f test.conf..."
./raftCoreRun -n 3 -f test.conf || { echo "Execution failed"; exit 1; }

echo "Script completed successfully."
