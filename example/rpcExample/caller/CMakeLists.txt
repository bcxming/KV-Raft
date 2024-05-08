# set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callFriendService.cpp ../friend.pb.cc)
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer rpc_lib)

# 查找 Protobuf 库
find_package(Protobuf REQUIRED)

# 添加包含 Protobuf 自动生成的头文件目录
include_directories(${PROTOBUF_INCLUDE_DIRS})

# 在您的目标中链接 Protobuf 库
target_link_libraries(consumer ${PROTOBUF_LIBRARIES})
