cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(This bj)

project(${This} CXX C)

find_package(Python3 COMPONENTS Interpreter Development NumPy)

include_directories(
    ${Python3_INCLUDE_DIRECTORIES}
    include
    /usr/include
)

add_executable(${PROJECT_NAME} main.cpp game.cpp game.hpp)
target_link_libraries(${This} ${Python3_LIBRARIES})
set(JSON_INC_PATH ~/Downloads/vcpkg/packages/matplotlib-cpp_x64-linux/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${JSON_INC_PATH})