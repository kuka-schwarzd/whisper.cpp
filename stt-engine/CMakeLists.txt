# Minimum CMake required
cmake_minimum_required(VERSION 3.15)

# dependencies

find_package(Threads REQUIRED)

# third-party

# SDL2
find_package(SDL2 REQUIRED)

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

message(STATUS "SDL2_INCLUDE_DIRS = ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES = ${SDL2_LIBRARIES}")

# common

set(TARGET common)

add_library(${TARGET} STATIC
    common.h
    common.cpp
    common-ggml.h
    common-ggml.cpp
    grammar-parser.cpp
    )

include(DefaultTargetOptions)

target_link_libraries(${TARGET} PRIVATE whisper)

set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# common-sdl

set(TARGET common-sdl)

add_library(${TARGET} STATIC
    common-sdl.h
    common-sdl.cpp
    )

include(DefaultTargetOptions)

target_include_directories(${TARGET} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})

set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "conv_agent_input_service.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/conv_agent_input_service.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/conv_agent_input_service.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/conv_agent_input_service.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/conv_agent_input_service.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Target
set(TARGET stt-engine)
add_executable(${TARGET} "${TARGET}.cpp"
  ${hw_proto_srcs}
  ${hw_grpc_srcs})
include(DefaultTargetOptions)
target_link_libraries(${TARGET}
  PRIVATE common common-sdl whisper -lcurl ${CMAKE_THREAD_LIBS_INIT}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${SDL2_LIBRARIES})
