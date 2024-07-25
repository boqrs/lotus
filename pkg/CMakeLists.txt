#if project is too large, cmakeList can set to sub model
cmake_minimum_required(VERSION 3.15)
project(pkg)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Source files: ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/pkg/include)

file(GLOB_RECURSE SRC_LIST ${CMAKE_SOURCE_DIR}/pkg/src/*.cpp)

if(NOT SRC_LIST)
    message(FATAL_ERROR "No source files found in ${CMAKE_SOURCE_DIR}/pkg/src")
endif()

add_library(pkg STATIC ${SRC_LIST})

target_link_libraries(pkg PRIVATE ${CMAKE_THREAD_LIBS_INIT})