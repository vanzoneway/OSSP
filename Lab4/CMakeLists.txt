cmake_minimum_required(VERSION 3.27)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab4 main.cpp
        message.h
        message.cpp
        shared_semaphores.cpp
        shared_semaphores.h)
