cmake_minimum_required(VERSION 3.29)
project(RPC_TCP)

set(CMAKE_CXX_STANDARD 14)

add_executable(RPC_TCP
        client.cpp
        server.cpp)
