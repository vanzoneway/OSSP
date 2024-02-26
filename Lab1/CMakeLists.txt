cmake_minimum_required(VERSION 3.27)
project(Lab1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Lab1 dirwalks.c
        dynamic_array.c
        dynamic_array.h
        recursive_reading.c
        recursive_reading.h
)
