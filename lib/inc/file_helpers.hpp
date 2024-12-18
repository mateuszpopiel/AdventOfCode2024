#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

std::ifstream open_file(const std::string_view &filename = "data.txt");
std::vector<std::string> file_to_string(const std::string_view &file);
std::string multiline_string_to_single_line(const std::vector<std::string> &input);
void print_file(const std::vector<std::string> &vec);

template <class T> auto parse_file(std::ifstream &file) -> std::vector<std::vector<T>> {
  std::vector<std::vector<T>> result;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::vector<T> row_data;
    T value;
    while (iss >> value) {
      row_data.push_back(value);
    }
    result.push_back(row_data);
  }
  return result;
}
