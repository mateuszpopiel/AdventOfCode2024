#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using LocationId = unsigned int;

unsigned int
get_similarity_score(const std::pair<std::vector<LocationId>, std::vector<LocationId>> &lists_of_locations) {
  auto left_list = lists_of_locations.first;
  auto right_list = lists_of_locations.second;

  unsigned int similarity_score = 0;
  std::for_each(left_list.begin(), left_list.end(), [&](LocationId left) {
    similarity_score += std::count(right_list.begin(), right_list.end(), left) * left;
  });
  return similarity_score;
}

int main() {
  std::ifstream file("data.txt");
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file." << std::endl;
    return 1;
  }

  std::vector<LocationId> left_list;
  std::vector<LocationId> right_list;

  LocationId left, right;
  while (file >> left >> right) {
    left_list.push_back(left);
    right_list.push_back(right);
  }

  std::cout << get_similarity_score(std::make_pair(left_list, right_list)) << std::endl;
  return 0;
}
