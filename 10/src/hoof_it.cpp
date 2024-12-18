#include "hoof_it.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <coordinates.hpp>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using Map = std::vector<std::vector<unsigned int>>;

auto file_to_string(std::ifstream &file) {
  std::string row;
  std::vector<std::string> file_as_string;
  while (std::getline(file, row)) {
    file_as_string.push_back(row);
  }
  return file_as_string;
}

auto file_as_string_to_map(const std::vector<std::string> &file_as_string) -> Map {
  auto map = Map(file_as_string.size(), std::vector<unsigned int>(file_as_string[0].size()));
  for (auto row = 0ull; row < file_as_string.size(); ++row) {
    for (auto column = 0ull; column < file_as_string[row].size(); ++column) {
      map[row][column] = static_cast<unsigned int>(file_as_string[row][column] - '0');
    }
  }
  return map;
}

auto get_all_trailheads(const Map &map) {
  auto trailheafs = std::vector<Coordinates<ull>>{};
  for (auto row = 0ull; row < map.size(); ++row) {
    for (auto column = 0ull; column < map[row].size(); ++column) {
      if (map[row][column] == 0) {
        trailheafs.emplace_back(row, column);
      }
    }
  }
  return trailheafs;
}

constexpr auto is_slope_valid(const ull height, const ull next_height) {
  return next_height > height && next_height - height == 1;
}

auto get_next_steps(const Map &map, const Coordinates<ull> &current_position) {
  auto steps = std::vector<Coordinates<ull>>{};
  const auto height = map[current_position.x][current_position.y];
  if (current_position.x > 0 && is_slope_valid(height, map[current_position.x - 1][current_position.y])) {
    steps.emplace_back(current_position.x - 1, current_position.y);
  }
  if (current_position.x < map.size() - 1 && is_slope_valid(height, map[current_position.x + 1][current_position.y])) {
    steps.emplace_back(current_position.x + 1, current_position.y);
  }
  if (current_position.y > 0 && is_slope_valid(height, map[current_position.x][current_position.y - 1])) {
    steps.emplace_back(current_position.x, current_position.y - 1);
  }
  if (current_position.y < map[current_position.x].size() - 1 &&
      is_slope_valid(height, map[current_position.x][current_position.y + 1])) {
    steps.emplace_back(current_position.x, current_position.y + 1);
  }
  return steps;
}

auto get_reachable_peaks(const Map &map, const Coordinates<ull> trailhead, std::vector<Coordinates<ull>> &peaks) {
  if (map[trailhead.x][trailhead.y] == 9) {
    peaks.push_back(trailhead);
    return;
  }
  const auto next_steps = get_next_steps(map, trailhead);
  for (const auto &next_step : next_steps) {
    get_reachable_peaks(map, next_step, peaks);
  }
}

void remove_repeated_peaks(std::vector<Coordinates<ull>> &peaks) {
  std::sort(peaks.begin(), peaks.end());
  peaks.erase(std::unique(peaks.begin(), peaks.end()), peaks.end());
}

auto get_sum_of_the_scores_of_all_trailheads(const Map &map, const bool part_2 = false) {
  ull sum = 0;
  const auto trailheads = get_all_trailheads(map);
  for (const auto &trailhead : trailheads) {
    std::vector<Coordinates<ull>> reachable_peaks{};
    get_reachable_peaks(map, trailhead, reachable_peaks);
    if (!part_2) {
      remove_repeated_peaks(reachable_peaks);
    }
    sum += reachable_peaks.size();
  }
  return sum;
}

auto get_map_from_file(const std::string &filename) -> Map { return file_as_string_to_map(file_to_string(filename)); }

ull solve(const bool part_2) {
  const auto topographic_map = get_map_from_file("data.txt");
  return get_sum_of_the_scores_of_all_trailheads(topographic_map, part_2);
}
