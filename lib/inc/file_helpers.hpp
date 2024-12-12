#pragma once
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

std::ifstream open_file(const std::string_view &filename = "data.txt");
std::vector<std::string> file_to_string(std::ifstream &file);
