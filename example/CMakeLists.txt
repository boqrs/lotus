cmake_minimum_required(VERSION 3.15)
project(example)

include_directories(${CMAKE_SOURCE_DIR}/pkg/include)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE pkg)

set_target_properties(exampleServer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/target
)