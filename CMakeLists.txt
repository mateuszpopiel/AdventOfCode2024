cmake_minimum_required(VERSION 3.28)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 20) # CI doesn't support C++23 yet
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wpedantic
  -Wconversion
  -Wsign-conversion
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
add_subdirectory(4)
add_subdirectory(5)
# TODO: Day 6 went wrong somewhere...
# add_subdirectory(6)
# TODO: Day 7 requires refactor, takes ~13s to execute
# add_subdirectory(7)
add_subdirectory(8)
add_subdirectory(9)
add_subdirectory(10)
add_subdirectory(11)
add_subdirectory(12)
add_subdirectory(13)
add_subdirectory(14)
add_subdirectory(15)
# TODO: Likely day 16 could have been done better
add_subdirectory(16)
add_subdirectory(17)
add_subdirectory(18)
add_subdirectory(19)
add_subdirectory(20)
