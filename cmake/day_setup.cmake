function(day_setup DAY THEME)
  add_library(${DAY}_lib STATIC src/${THEME}.cpp)
  target_include_directories(${DAY}_lib PUBLIC inc)
  target_link_libraries(${DAY}_lib PRIVATE file_helpers)

  if (NOT ENABLE_TESTS)
    add_executable(${DAY} src/main.cpp)
    copy_data_file(${DAY} ${CMAKE_CURRENT_SOURCE_DIR}/resources/data.txt)
    target_link_libraries(${DAY} PRIVATE ${DAY}_lib file_helpers)
  else()
    add_executable(test_${DAY} tests/test_${DAY}.cpp)
    target_link_libraries(test_${DAY} PRIVATE ${DAY}_lib file_helpers gtest gtest_main)
    copy_data_file(test_${DAY} ${CMAKE_CURRENT_SOURCE_DIR}/resources/data.txt)
    add_test(NAME test_${DAY} COMMAND $<TARGET_FILE:test_${DAY}>)
  endif()
endfunction()