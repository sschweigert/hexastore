cmake_minimum_required(VERSION 2.8.9)
project(hexastore)

include_directories(include)

add_executable(main src/main.cpp src/datastore.cpp src/triangle_detection.cpp src/hexastore.cpp src/datatypes.cpp src/output.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

add_executable(test test/test.cpp src/datastore.cpp src/triangle_detection.cpp src/hexastore.cpp src/datatypes.cpp src/output.cpp)
