cmake_minimum_required(VERSION 3.15)
project(api-example)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
include(FetchContent)
FetchContent_Declare(
        grpc
        GIT_REPOSITORY https://github.com/grpc/grpc.git
        GIT_TAG v1.26.0)
FetchContent_MakeAvailable(grpc)

# Since FetchContent uses add_subdirectory under the hood, we can use
# the grpc targets directly from this build.
set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)

if (CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else ()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
endif ()

# Proto file
get_filename_component(hw_proto "protos/api.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/api.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/api.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}"
)

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_executable(api-example main.cpp
        ${hw_proto_srcs}
        ${hw_grpc_srcs})

target_link_libraries(api-example
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF})