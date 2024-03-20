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
    common/common.h
    common/common.cpp
    common/common-ggml.h
    common/common-ggml.cpp
    grammar-parser/grammar-parser.cpp
    )

include(DefaultTargetOptions)

target_link_libraries(${TARGET} PRIVATE whisper)

set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# common-sdl
set(TARGET common-sdl)

add_library(${TARGET} STATIC
    common/common-sdl.h
    common/common-sdl.cpp
    )

include(DefaultTargetOptions)

target_include_directories(${TARGET} PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})

set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Proto directory
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/conv-agent/proto-api/conv-agent-api/proto/kuka/conv_agent)
set(PROTO_OUT ${CMAKE_CURRENT_BINARY_DIR}/conv-agent)
file(MAKE_DIRECTORY ${PROTO_OUT})

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
get_filename_component(hw_proto "${PROTO_DIR}/conv_agent_input_service.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${PROTO_OUT}/conv_agent_input_service.pb.cc")
set(hw_proto_hdrs "${PROTO_OUT}/conv_agent_input_service.pb.h")
set(hw_grpc_srcs "${PROTO_OUT}/conv_agent_input_service.grpc.pb.cc")
set(hw_grpc_hdrs "${PROTO_OUT}/conv_agent_input_service.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${PROTO_OUT}"
        --cpp_out "${PROTO_OUT}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# conv-agent
set(TARGET conv-agent)

add_library(${TARGET} STATIC
    conv-agent/conv-agent-client.h
    conv-agent/conv-agent-client.cpp
    ${hw_proto_srcs}
    ${hw_grpc_srcs}
)

include(DefaultTargetOptions)

target_link_libraries(${TARGET} PRIVATE ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF}
)

set_target_properties(${TARGET} PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Target
set(TARGET stt-engine)
add_executable(${TARGET} "${TARGET}.cpp")
include(DefaultTargetOptions)
target_link_libraries(${TARGET}
  PRIVATE common common-sdl whisper conv-agent ${CMAKE_THREAD_LIBS_INIT} ${SDL2_LIBRARIES} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF}
)
