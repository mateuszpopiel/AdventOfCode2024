#include "linean_layout.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>> get_towels(const std::string &filename) {
  const auto file_str = file_to_string(filename);
  auto towels_str = file_str.front();
  std::replace(towels_str.begin(), towels_str.end(), ',', ' ');
  std::unordered_set<std::string> towels;
  std::istringstream towels_iss(towels_str);
  while (towels_iss >> towels_str) {
    towels.insert(towels_str);
  }
  const std::unordered_set<std::string> designs = {file_str.begin() + 2, file_str.end()};
  return std::make_pair(towels, designs);
}

ull count_ways_to_make_design(const std::unordered_set<std::string> &towels, const std::string &design) {
  // The idea is to count on how many ways part of the design could be made
  std::vector<ull> design_part(design.size() + 1, 0);
  design_part[0] = 1;

  for (size_t i = 1; i <= design.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      if (towels.contains(design.substr(j, i - j))) {
        design_part[i] += design_part[j];
      }
    }
  }

  return design_part[design.size()];
}

ull count_possible_designs(const std::unordered_set<std::string> &towels,
                           const std::unordered_set<std::string> &designs) {
  return static_cast<ull>(std::count_if(designs.begin(), designs.end(), [&towels](const std::string &design) {
    return count_ways_to_make_design(towels, design) > 0;
  }));
}

ull count_ways_to_make_designs(const std::unordered_set<std::string> &towels,
                               const std::unordered_set<std::string> &designs) {
  return std::accumulate(designs.begin(), designs.end(), 0ULL, [&towels](ull acc, const std::string &design) {
    return acc + count_ways_to_make_design(towels, design);
  });
}

ull solve(const bool part_2) {
  const auto [towels, designs] = get_towels("data.txt");
  if (!part_2) {
    return count_possible_designs(towels, designs);
  }
  return count_ways_to_make_designs(towels, designs);
}
