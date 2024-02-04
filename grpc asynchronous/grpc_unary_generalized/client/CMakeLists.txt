
cmake_minimum_required(VERSION 3.22.1)

project(GeneralizedGRPCFrontend C CXX)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")
# include_directories("../build/")

add_executable(mx_client client.h client.cpp)

target_link_libraries(mx_client
rg_grpc_proto)