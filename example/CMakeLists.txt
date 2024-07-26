cmake_minimum_required(VERSION 3.15)
project(example)

include_directories(${CMAKE_SOURCE_DIR}/pkg/include)

file(GLOB EXAMPLE_SRC ${CMAKE_SOURCE_DIR}/example/*.cpp)

add_executable(exampleServer ${EXAMPLE_SRC})
target_link_libraries(exampleServer PRIVATE pkg)
set_target_properties(exampleServer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/target
)