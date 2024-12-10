#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>

static constexpr std::string_view filename = "data.txt";

using Map = std::vector<std::vector<unsigned int>>;

struct Coordinates {
  size_t x;
  size_t y;

  constexpr Coordinates(size_t x, size_t y) : x(x), y(y) {}

  constexpr Coordinates operator+(const Coordinates &other) const {
    return {x + other.x, y + other.y};
  }

  constexpr bool operator<(const Coordinates &other) const {
    if (x < other.x) return true;
    if (x > other.x) return false;
    return y < other.y;
  }

  constexpr bool operator==(const Coordinates &other) const {
    return x == other.x && y == other.y;
  }
};

auto open_file(const std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_string(std::ifstream &file) {
  std::string row;
  std::vector<std::string> file_as_string;
  while (std::getline(file, row)) {
    file_as_string.push_back(row);
  }
  return file_as_string;
}

auto file_as_string_to_map(const std::vector<std::string> &file_as_string) {
  auto map = Map(file_as_string.size(), std::vector<unsigned int>(file_as_string[0].size()));
  for (size_t row = 0; row < file_as_string.size(); ++row) {
    for (size_t column = 0; column < file_as_string[row].size(); ++column) {
      map[row][column] = file_as_string[row][column] - '0';
    }
  }
  return map;
}

auto get_all_trailheads(const Map &map) {
  auto trailheafs = std::vector<Coordinates>{};
  for (size_t row = 0; row < map.size(); ++row) {
    for (size_t column = 0; column < map[row].size(); ++column) {
      if (map[row][column] == 0) {
        trailheafs.push_back({row, column});
      }
    }
  }
  return trailheafs;
}

constexpr auto is_slope_valid(const size_t height, const size_t next_height) {
  return next_height > height && next_height - height == 1;
}

auto get_next_steps(const Map &map, const Coordinates &current_position) {
  auto steps = std::vector<Coordinates>{};
  const auto height = map[current_position.x][current_position.y];
  if (current_position.x > 0 && is_slope_valid(height, map[current_position.x - 1][current_position.y])) {
    steps.push_back({current_position.x - 1, current_position.y});
  }
  if (current_position.x < map.size() - 1 && is_slope_valid(height, map[current_position.x + 1][current_position.y])) {
    steps.push_back({current_position.x + 1, current_position.y});
  }
  if (current_position.y > 0 && is_slope_valid(height, map[current_position.x][current_position.y - 1])) {
    steps.push_back({current_position.x, current_position.y - 1});
  }
  if (current_position.y < map[current_position.x].size() - 1 && is_slope_valid(height, map[current_position.x][current_position.y + 1])) {
    steps.push_back({current_position.x, current_position.y + 1});
  }
  return steps;
}

auto get_reachable_peaks(const Map &map, const Coordinates trailhead, std::vector<Coordinates> &peaks) {
  if (map[trailhead.x][trailhead.y] == 9) {
    peaks.push_back(trailhead);
    return;
  }
  const auto next_steps = get_next_steps(map, trailhead);
  for (const auto &next_step : next_steps) {
    get_reachable_peaks(map, next_step, peaks);
  }
}

void remove_repeated_peaks(std::vector<Coordinates> &peaks) {
  std::sort(peaks.begin(), peaks.end());
  peaks.erase(std::unique(peaks.begin(), peaks.end()), peaks.end());
}

auto get_sum_of_the_scores_of_all_trailheads(const Map &map, const bool part_2 = false) {
  size_t sum = 0;
  const auto trailheads = get_all_trailheads(map);
  for (const auto &trailhead : trailheads) {
    std::vector<Coordinates> reachable_peaks {};
    get_reachable_peaks(map, trailhead, reachable_peaks);
    if (!part_2) remove_repeated_peaks(reachable_peaks);
    sum += reachable_peaks.size();
  }
  return sum;
}

int main() {
  auto file = open_file(filename);
  const auto file_as_string = file_to_string(file);
  const auto topographic_map = file_as_string_to_map(file_as_string);
  std::cout << get_sum_of_the_scores_of_all_trailheads(topographic_map) << '\n'; // 709
  std::cout << get_sum_of_the_scores_of_all_trailheads(topographic_map, true) << '\n'; // 1326
  return 0;
}
