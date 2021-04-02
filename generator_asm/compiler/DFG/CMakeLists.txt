cmake_minimum_required(VERSION 3.16)
project(dfg)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fno-rtti" )

include_directories(${HEADER_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE H_SRCS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE CXX_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(DFG MODULE ${CXX_SRCS})

target_link_libraries(DFG ${HEADER_INCLUDE_DIR})

configure_file(script/LoopParser.py ${CMAKE_BINARY_DIR}/script/LoopParser.py COPYONLY)