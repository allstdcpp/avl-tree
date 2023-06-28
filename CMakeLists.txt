cmake_minimum_required(VERSION 3.26)
project(avl-tree)

set(CMAKE_CXX_STANDARD 20)

add_executable(avl-tree main.cpp)
target_link_libraries(avl-tree PRIVATE -fsanitize=undefined,address)
target_compile_options(avl-tree PRIVATE -fsanitize=undefined,address)
