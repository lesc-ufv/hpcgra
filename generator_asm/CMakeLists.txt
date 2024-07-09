cmake_minimum_required(VERSION 3.5)

project(Graph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O3")

file(GLOB_RECURSE CXX_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(place ${CXX_SRCS})

target_include_directories(place PRIVATE ${CMAKE_SOURCE_DIR}/include)

