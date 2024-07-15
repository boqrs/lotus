cmake_minimum_required(VERSION 3.15)
project(pkg)

aux_source_directory(./src SRC) #may be need add dir
add_library(pkg ${SRC}
        src/utils/Logger.cpp)

