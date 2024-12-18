#include "../inc/file_helpers.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

auto open_file(const std::string_view &filename) -> std::ifstream {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto ifstream_file_to_string(std::ifstream &file) -> std::vector<std::string> {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}

auto file_to_string(const std::string_view &filename) -> std::vector<std::string> {
  auto file = open_file(filename);
  auto lines = ifstream_file_to_string(file);
  file.close();
  return lines;
}

auto multiline_string_to_single_line(const std::vector<std::string> &input) -> std::string {
  std::string single_line;
  for (const auto &line : input) {
    single_line += line + ' ';
  }
  return single_line;
}

void print_file(const std::vector<std::string> &vec) {
  for (const auto &line : vec) {
    std::cout << line << '\n';
  }
}
