cmake_minimum_required(VERSION 3.10)
project(example)

message(STATUS "Source files: ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/pkg/include)

file(GLOB EXAMPLE_SRC ${CMAKE_SOURCE_DIR}/example/*.cpp)

message(STATUS "Example source files: ${EXAMPLE_SRC}")

add_executable(ExampleExecutable ${EXAMPLE_SRC})

# 将静态库链接到可执行文件
target_link_libraries(ExampleExecutable PRIVATE pkg)