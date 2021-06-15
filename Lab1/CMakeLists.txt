cmake_minimum_required(VERSION 3.19)
project(Lab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(linker tokenizer.hpp main.cpp)
