cmake_minimum_required(VERSION 2.8.9)
project(hexastore)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

add_library(hexastore_lib
	src/datastore.cpp 
	src/triangle_detection.cpp 
	src/hexastore.cpp 
	src/datatypes.cpp 
	src/output.cpp
)

add_executable(main src/main.cpp )

add_executable(test test/test.cpp)

target_link_libraries(main hexastore_lib)
target_link_libraries(test hexastore_lib)
