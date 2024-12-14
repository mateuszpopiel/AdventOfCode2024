# Copy FILE_NAME to build directory
function(copy_data_file TARGET_NAME FILE_NAME)
  if (NOT EXISTS ${FILE_NAME})
    message(FATAL_ERROR "File ${FILE_NAME} does not exist")
  endif()

  add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${FILE_NAME}
            $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()

