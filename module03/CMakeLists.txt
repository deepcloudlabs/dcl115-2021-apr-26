cmake_minimum_required(VERSION 3.19)
project(module03)
project(exercise01)
project(dining_philosophers)
project(study-thread-local)
project(study-once-call-flag)

set(CMAKE_CXX_STANDARD 17)

add_executable(module03 main.cpp)
add_executable(exercise01 exercise01.cpp)
add_executable(dining_philosophers dining_philosophers.cpp)
add_executable(study-thread-local study-thread-local.cpp)
add_executable(study-once-call-flag study-once-call-flag.cpp)