cmake_minimum_required(VERSION 3.15)
project(example)

message(STATUS "Source files: ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/pkg/include)

file(GLOB EXAMPLE_SRC ${CMAKE_SOURCE_DIR}/example/*.cpp)

message(STATUS "Example source files: ${EXAMPLE_SRC}")


target_link_libraries(ExampleExecutable PRIVATE pkg)

add_executable(ExampleExecutable ${EXAMPLE_SRC})

set_target_properties(ExampleExecutable PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/target
)