cmake_minimum_required(VERSION 3.13.0)

project(WeTalk)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# -Werror
add_compile_options(-Wall -Wextra)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -std=c++17")

add_subdirectory(third_party)
add_subdirectory(src)
