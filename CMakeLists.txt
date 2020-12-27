cmake_minimum_required(VERSION 3.19.1)

project(GraphAlgorithms)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(inputFiles)
add_subdirectory(apps)

enable_testing()
add_subdirectory(tests)
