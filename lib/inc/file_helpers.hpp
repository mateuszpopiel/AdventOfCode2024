#pragma once
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

std::ifstream open_file(const std::string_view &filename = "data.txt");
std::vector<std::string> file_to_string(std::ifstream &file);
std::vector<std::string> get_input_from_multiline_file(const std::string_view &filename = "data.txt");
std::string multiline_string_to_single_line(const std::vector<std::string> &input);
void print_file(const std::vector<std::string> &vec);
