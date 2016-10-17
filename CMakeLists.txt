cmake_minimum_required(VERSION 2.8.9)
project(hexastore)

include_directories(include)

add_executable(main src/main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
