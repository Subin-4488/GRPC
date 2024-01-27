cmake_minimum_required(VERSION 3.22.1)

project(Greet C CXX)

add_subdirectory(server)
add_subdirectory(client)

  include("common.cmake")

  # Proto file
  get_filename_component(rg_proto "protos/mx.proto" ABSOLUTE)
  get_filename_component(rg_proto_path "${rg_proto}" PATH)

  set(rg_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/mx.pb.cc")
  set(rg_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mx.pb.h")
  set(rg_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/mx.grpc.pb.cc")
  set(rg_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/mx.grpc.pb.h")
  add_custom_command(
        OUTPUT "${rg_proto_srcs}" "${rg_proto_hdrs}" "${rg_grpc_srcs}" "${rg_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
          --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
          -I "${rg_proto_path}" 
          --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
          "${rg_proto}"
        DEPENDS "${rg_proto}")

  # Include generated *.pb.h files
  # rg_grpc_proto
  add_library(rg_grpc_proto SHARED
    ${rg_grpc_srcs}
    ${rg_grpc_hdrs}
    ${rg_proto_srcs}
    ${rg_proto_hdrs})
  target_link_libraries(rg_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})