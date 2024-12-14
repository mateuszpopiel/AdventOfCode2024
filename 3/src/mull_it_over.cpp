#include "mull_it_over.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

auto convert_file_to_one_line_string(std::ifstream &file) {
  std::ostringstream buffer;
  std::string line;
  while (std::getline(file, line)) {
    buffer << line;
  }
  return buffer.str();
}

auto get_file_as_string(const std::string_view &filename) {
  auto file = open_file(filename);
  return convert_file_to_one_line_string(file);
}

std::string get_data(const std::string &filename) {
  auto file = open_file(filename);
  auto file_as_string = convert_file_to_one_line_string(file);
  file.close();
  return file_as_string;
}

void filter_string(std::string &file_as_str) {
  while (file_as_str.find("don't") != std::string::npos) {
    const std::size_t start = file_as_str.find("don't()");
    if (start == std::string::npos) {
      break;
    }
    const std::size_t end = file_as_str.find("do()", start);
    if (end == std::string::npos) {
      file_as_str = file_as_str.substr(0, start);
      break;
    }
    file_as_str = file_as_str.substr(0, start) + file_as_str.substr(end, file_as_str.size() - end);
  }
}

ull mul_string_over(const std::string &line) {
  std::string line_copy = line;
  ull value = 0;
  while (line_copy.find("mul") != std::string::npos) {
    const std::size_t start = line_copy.find("mul(") + 4;
    const std::size_t end = line_copy.find(')', start);
    std::string mul_content = line_copy.substr(start, end - start);
    if (std::count(mul_content.begin(), mul_content.end(), ',') != 1 ||
        !std::all_of(mul_content.begin(), mul_content.end(),
                     [](char c) { return (std::isdigit(c) != 0) || c == ' ' || c == ','; })) {
      line_copy = line_copy.substr(start);
      continue;
    }
    std::replace(mul_content.begin(), mul_content.end(), ',', ' ');

    std::istringstream iss(mul_content);
    ull x = 0;
    ull y = 0;
    iss >> x >> y;
    value += x * y;
    line_copy = line_copy.substr(end + 1);
  }
  return value;
}

ull mul_it_over(std::string &file_as_string, const bool part_2) {
  if (part_2) {
    filter_string(file_as_string);
  }
  return mul_string_over(file_as_string);
}

ull solve(const bool part_2) {
  auto file_as_str = get_file_as_string("data.txt");
  return mul_it_over(file_as_str, part_2);
}
