cmake_minimum_required(VERSION 3.28)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 20) # CI doesn't support C++23 yet
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wno-unused-command-line-argument
  -O3
  -fstack-usage
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(copy_data_file)
include(day_setup)

if (ENABLE_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
  # Use clang tidy only with test builds
  set(CLANG_TIDY_FILE ${CMAKE_SOURCE_DIR}/.clang-tidy)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-config-file=${CLANG_TIDY_FILE}")
endif()

add_subdirectory(lib)
add_subdirectory(0)
add_subdirectory(1)
add_subdirectory(2)
add_subdirectory(3)
