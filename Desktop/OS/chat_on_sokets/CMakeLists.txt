cmake_minimum_required(VERSION 3.29)
project(chat_on_sokets)

set(CMAKE_CXX_STANDARD 14)

add_executable(chat_on_sokets
        client.cpp
        server.cpp)
