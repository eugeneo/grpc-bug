cmake_minimum_required(VERSION 3.21)
project(grpc_bug)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This submodule assumes that gRPC and all its dependencies are already installed
# on this system, so they can be located by find_package().

find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(helloProto STATIC
    hello.proto
)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(
    TARGET helloProto
    LANGUAGE cpp
    PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gen
)
protobuf_generate(
    TARGET helloProto
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
    PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gen
)
# include generated headers
target_include_directories(helloProto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gen>
        $<INSTALL_INTERFACE:include/eservicemsg>
)
target_link_libraries(helloProto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
set_target_properties(helloProto PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_compile_options(helloProto PUBLIC -fsanitize=address)
target_link_options(helloProto PUBLIC -fsanitize=address)


add_executable(client
    src/client.cpp
)
add_executable(server
    src/server.cpp
)
list(APPEND apps client server)
foreach(app IN LISTS apps)
    target_link_libraries(${app}
    PUBLIC
        helloProto
        Threads::Threads
        atomic
    )
    target_include_directories(${app}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include>
        $<INSTALL_INTERFACE:include>
    )
    set_target_properties(${app} PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
        OUTPUT_NAME ${app}
    )
    target_compile_options(${app} PUBLIC -fsanitize=address)
    target_link_options(${app} PUBLIC -fsanitize=address)
endforeach()
