cmake_minimum_required(VERSION 3.8)

project(ChatServer C CXX)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")

add_executable(server chatserver.h chatserver.cpp)

target_link_libraries(server rg_grpc_proto)