cmake_minimum_required(VERSION 3.19.1)

project(GraphAlgorithms)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(tests)
add_subdirectory(apps)