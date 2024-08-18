cmake_minimum_required(VERSION 3.30)
project(inx-engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(inx)
add_subdirectory(sandbox)