#include "template.hpp"
#include "file_helpers.hpp"
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<ull>> get_data(const std::string &filename) {
  std::vector<std::vector<ull>> result;
  const auto content = get_input_from_multiline_file(filename);
  for (const auto &row : content) {
    auto iss = std::istringstream(row);
    std::vector<ull> row_data;
    ull value = 0;
    while (iss >> value) {
      row_data.push_back(value);
    }
    result.push_back(row_data);
  }
  return result;
}

ull solve() {
  const auto data = get_data("data.txt");
  return (data[0][0] + data[0][1]) * (data[1][0] - data[1][1]);
}
