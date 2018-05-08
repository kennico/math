cmake_minimum_required(VERSION 3.10)
project(math)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test/numa)
add_subdirectory(test/numt)
add_subdirectory(src)