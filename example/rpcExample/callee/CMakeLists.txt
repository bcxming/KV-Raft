
set(SRC_LIST friendService.cpp ../friend.pb.cc)


add_executable(provider ${SRC_LIST})
target_link_libraries(provider rpc_lib muduo_net muduo_base pthread)
# 查找 Protobuf 库
find_package(Protobuf REQUIRED)

# 添加包含 Protobuf 自动生成的头文件目录
include_directories(${PROTOBUF_INCLUDE_DIRS})

# 在您的目标中链接 Protobuf 库
target_link_libraries(provider ${PROTOBUF_LIBRARIES})

