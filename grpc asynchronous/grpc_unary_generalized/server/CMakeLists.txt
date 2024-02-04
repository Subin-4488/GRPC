cmake_minimum_required(VERSION 3.22.1)

project(GeneralizedGRPCBackend C CXX)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")

add_executable(mx_server calldata_interface.h server.h server.cpp)

target_link_libraries(mx_server
rg_grpc_proto
)