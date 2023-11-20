cmake_minimum_required(VERSION 3.0)

project(PJ)

include_directories(include)

add_executable(main_cmake main.cpp src/huffman.cpp src/function.cpp)