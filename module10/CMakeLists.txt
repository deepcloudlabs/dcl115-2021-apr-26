cmake_minimum_required(VERSION 3.19)
project(module10)
project(range-exercise01)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread -fopenmp")
set(CMAKE_CXX_STANDARD 20)

add_executable(module10 main.cpp world/city.cpp world/city.h world/country.h world/country.cpp world/world-util.h world/world-util.cpp)
add_executable(range-exercise01 range/exercise1.cpp ../world/city.cpp ../world/city.h ../world/country.h ../world/country.cpp ../world/world-util.h ../world/world-util.cpp)