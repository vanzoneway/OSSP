cmake_minimum_required(VERSION 3.27)
project(Lab5_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(Lab5_1 main.cpp
        message.cpp
        message.h)
