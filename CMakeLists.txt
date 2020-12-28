cmake_minimum_required(VERSION 3.19.1)

project(GraphAlgorithms CXX)

set(CMAKE_CXX_STANDARD 17)
set(FILE_PATH "${PROJECT_SOURCE_DIR}/graphs" CACHE FILEPATH "Full path of the file")

include_directories(include)
add_subdirectory(apps)

enable_testing()
add_subdirectory(tests)
