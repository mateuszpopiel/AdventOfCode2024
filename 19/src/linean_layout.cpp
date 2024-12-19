#include "linean_layout.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::pair<std::vector<std::string>, std::vector<std::string>> get_towels(const std::string &filename) {
  const auto file_str = file_to_string(filename);
  auto towels_str = file_str.front();
  std::replace(towels_str.begin(), towels_str.end(), ',', ' ');
  std::vector<std::string> towels;
  std::istringstream towels_iss(towels_str);
  while (towels_iss >> towels_str) {
    towels.push_back(towels_str);
  }
  const std::vector<std::string> designs = {file_str.begin() + 2, file_str.end()};
  return std::make_pair(towels, designs);
}

bool is_design_possible(const std::vector<std::string> &towels, const std::string &design) {
  // stores whether a design of length i can be formed
  std::vector<bool> design_part(design.size() + 1, false);
  design_part[0] = true; // Base case: empty design can always be formed

  for (auto i = 1u; i <= design.size(); ++i) {
    for (const auto &towel : towels) {
      auto len = towel.size();
      if (i >= len && design.substr(i - len, len) == towel && design_part[i - len]) {
        design_part[i] = true;
        break;
      }
    }
  }

  return design_part[design.size()];
}

// NOLINTNEXTLINE(*-easily-swappable-parameters)
ull count_possible_designs(const std::vector<std::string> &towels, const std::vector<std::string> &designs) {
  ull count = 0;
  for (const auto &design : designs) {
    if (is_design_possible(towels, design)) {
      ++count;
    }
  }
  return count;
}

ull solve(const bool /*part_2*/) {
  const auto [towels, designs] = get_towels("/home/mateusz/workspace/AdventOfCode2024/19/resources/data.txt");
  return count_possible_designs(towels, designs);
}
