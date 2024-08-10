cmake_minimum_required(VERSION 3.30)
project(inx-engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dep)
add_subdirectory(src)