cmake_minimum_required(VERSION 3.28)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(copy_data_file)

add_subdirectory(1)
