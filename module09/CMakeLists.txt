cmake_minimum_required(VERSION 3.19)
project(module09)

set(CMAKE_CXX_STANDARD 17)

add_executable(module09 main.cpp threadsafe_queue.cpp threadsafe_queue.h)