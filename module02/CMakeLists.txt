cmake_minimum_required(VERSION 3.19)
project(module02)
project(exercise1)
project(exercise2)
project(exercise3)
project(study-naive-data-parallelism)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fopenmp")

add_executable(module02 main.cpp thread_guard.cpp thread_guard.h)
add_executable(exercise1 exercise1.cpp)
add_executable(exercise2 exercise2.cpp)
add_executable(exercise3 exercise3.cpp thread_guard.cpp thread_guard.h)
add_executable(study-naive-data-parallelism study-naive-data-parallelism.cpp)
