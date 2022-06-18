cmake_minimum_required(VERSION 3.0)

project(Graph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -O3")

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE H_SRCS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE CXX_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(place ${CXX_SRCS} ${H_SRCS})

target_link_libraries(place)
