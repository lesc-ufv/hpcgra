cmake_minimum_required(VERSION 3.0)

project(poly5)

set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(${CMAKE_SOURCE_DIR}/../../include ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE DF_H_SRCS ${CMAKE_SOURCE_DIR}/../../include/*.h)
file(GLOB_RECURSE DF_CXX_SRCS ${CMAKE_SOURCE_DIR}/../../src/*.cpp)

file(GLOB CPP_SRCS *.cpp)
file(GLOB H_SRCS *.h)

add_library(dataflow SHARED ${DF_CXX_SRCS} ${DF_H_SRCS})

add_executable(poly5 ${CPP_SRCS} ${H_SRCS})

target_link_libraries(poly5 dataflow)