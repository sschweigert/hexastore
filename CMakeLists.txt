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
	src/query_chain.cpp
	src/dataset.cpp
)

add_executable(main src/main.cpp )
target_link_libraries(main hexastore_lib)

add_executable(timing src/timing.cpp)
target_link_libraries(timing hexastore_lib)

add_executable(test test/test.cpp)
target_link_libraries(test hexastore_lib)
