cmake_minimum_required(VERSION 3.8)

project(ChatClient C CXX)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")

add_executable(client chatclient.h chatclient.cpp)
target_link_libraries(client rg_grpc_proto)