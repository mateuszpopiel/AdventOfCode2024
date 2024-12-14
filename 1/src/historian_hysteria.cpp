#include "file_helpers.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

using LocationId = unsigned int;

unsigned int get_total_distance(const std::pair<std::vector<LocationId>, std::vector<LocationId>> &lists_of_locations) {
  if (lists_of_locations.first.size() != lists_of_locations.second.size()) {
    throw std::invalid_argument("Lists must have the same size");
  }

  auto left_list = lists_of_locations.first;
  auto right_list = lists_of_locations.second;

  std::sort(left_list.begin(), left_list.end());
  std::sort(right_list.begin(), right_list.end());

  unsigned int distance = 0;
  for (size_t i = 0; i < left_list.size(); ++i) {
    distance += static_cast<unsigned int>(std::abs(static_cast<int>(left_list[i]) - static_cast<int>(right_list[i])));
  }
  return distance;
}

unsigned int
get_similarity_score(const std::pair<std::vector<LocationId>, std::vector<LocationId>> &lists_of_locations) {
  auto left_list = lists_of_locations.first;
  auto right_list = lists_of_locations.second;

  unsigned int similarity_score = 0;
  std::for_each(left_list.begin(), left_list.end(), [&](LocationId left) {
    similarity_score += static_cast<unsigned int>(std::count(right_list.begin(), right_list.end(), left) * left);
  });
  return similarity_score;
}

unsigned int solve(bool part_2) {
  auto file = open_file();

  std::vector<LocationId> left_list;
  std::vector<LocationId> right_list;

  LocationId left = 0;
  LocationId right = 0;
  while (file >> left >> right) {
    left_list.push_back(left);
    right_list.push_back(right);
  }
  if (!part_2) {
    return get_total_distance(std::make_pair(left_list, right_list));
  }
  return get_similarity_score(std::make_pair(left_list, right_list));
}
