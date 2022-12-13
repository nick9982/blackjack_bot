cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(This bj)

project(${This} CXX C)

include_directories(
    include
    /usr/include
)

add_executable(${PROJECT_NAME} main.cpp game.cpp game.hpp)