cmake_minimum_required(VERSION 3.15)
project(lotus)
set(CMAKE_CXX_STANDARD  11)

include_directories(${PROJECT_SOURCE_DIR}/pkg/include)

add_subdirectory(comm)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/target)
link_libraries(comm)
add_executable(app main.cpp)