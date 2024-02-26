cmake_minimum_required(VERSION 3.27)
project(parent)

set(CMAKE_CXX_STANDARD 20)

add_executable(parent parent.cpp)
add_executable(child child/child.cpp)

