cmake_minimum_required(VERSION 3.9)
project(LabOS C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(LabOS main.c param.h)
add_executable(Tool tool/main.c param.h)
add_executable(Calc calc/main.c param.h)