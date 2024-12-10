add_executable(historian_hysteria historian_hysteria.cpp)
copy_data_file(historian_hysteria ${CMAKE_CURRENT_SOURCE_DIR}/data.txt)
