#include "template.hpp"
#include "file_helpers.hpp"
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<ull>> get_data(const std::string &filename) {
  auto file = open_file(filename);
  const auto result = parse_file<ull>(file);
  file.close();
  return result;
}

ull solve(const bool part_2) {
  const auto data = get_data("data.txt");
  if (!part_2) {
    return (data[0][0] + data[0][1]) * (data[1][0] - data[1][1]);
  }
  return (data[0][0] - data[0][1]) + (data[1][0] * data[1][1]);
}
