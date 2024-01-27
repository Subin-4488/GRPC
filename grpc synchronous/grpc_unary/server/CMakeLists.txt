  cmake_minimum_required(VERSION 3.22.1)

  project(GreetBackend C CXX)

  include_directories("${CMAKE_CURRENT_BINARY_DIR}/..")
  # include_directories("../build/")

  add_executable(mx_server server.h server.cpp)
  
  target_link_libraries(mx_server
  rg_grpc_proto
  )