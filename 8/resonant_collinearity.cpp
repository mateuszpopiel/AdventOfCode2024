#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <sstream>
#include <array>
#include <string>
#include <utility>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <cmath>

static constexpr std::string_view filename {"data.txt"};

using Map = std::vector<std::string>;
using Coordinates = std::pair<long long, long long>;

auto open_file(const std::string_view &filename = filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_string(std::ifstream &file) {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}
auto get_frequencies(const Map &city_map) {
  std::string frequencies;
  for (const auto &line : city_map) {
    for (const auto &c : line) {
      if ((std::isdigit(c) || std::isalpha(c)) && (frequencies.find(c) == frequencies.npos)) frequencies.push_back(c);
    }
  }
  return frequencies;
}

auto get_same_frequency_antennas_coordinates(const Map &city_map, const char frequency) {
  std::vector<Coordinates> antennas;
  for (size_t i = 0; i < city_map.size(); ++i) {
    for (size_t j = 0; j < city_map[i].size(); ++j) {
      if (city_map[i][j] == frequency) {
        antennas.push_back({i, j});
      }
    }
  }
  return antennas;
}

auto compute_vector(const Coordinates &a, const Coordinates &b) {
  return std::make_pair(b.first - a.first, b.second - a.second);
}

auto get_antinodes_for_two_antennas(
  const Coordinates &antenna1, const Coordinates &antenna2, const Map &city_map) {

  std::vector<Coordinates> antinodes;
  const auto frequency = city_map[antenna1.first][antenna1.second];
  const auto vector_coordinates = compute_vector(antenna2, antenna1);
  const auto potential_antinode1 =
    std::make_pair(antenna1.first + vector_coordinates.first, antenna1.second + vector_coordinates.second);
  const auto potential_antinode2 =
    std::make_pair(antenna2.first - vector_coordinates.first, antenna2.second - vector_coordinates.second);
  if (potential_antinode1.first >= 0 && static_cast<size_t>(potential_antinode1.first) < city_map.size() &&
      potential_antinode1.second >= 0 && static_cast<size_t>(potential_antinode1.second) < city_map[0].size() &&
      (city_map[potential_antinode1.first][potential_antinode1.second] != frequency)) {
    antinodes.push_back(potential_antinode1);
  }
  if (potential_antinode2.first >= 0 && static_cast<size_t>(potential_antinode2.first) < city_map.size() &&
      potential_antinode2.second >= 0 && static_cast<size_t>(potential_antinode2.second) < city_map[0].size() &&
      (city_map[potential_antinode2.first][potential_antinode2.second] != frequency)) {
    antinodes.push_back(potential_antinode2);
  }
  return antinodes;
}

auto get_antinodes_for_frequency(const Map &city_map, const char frequency) {
  const auto antennas = get_same_frequency_antennas_coordinates(city_map, frequency);
  std::vector<Coordinates> antinodes;
  for (auto antenna1_iter = antennas.begin(); antenna1_iter != std::prev(antennas.end()); ++antenna1_iter) {
    for (auto antenna2_iter = std::next(antenna1_iter); antenna2_iter != antennas.end(); ++antenna2_iter) {
      const auto new_antinodes = get_antinodes_for_two_antennas(*antenna1_iter, *antenna2_iter, city_map);
      for (const auto &antinode : new_antinodes) {
        antinodes.push_back(antinode);
      }
    }
  }
  return antinodes;
}

void remove_duplicates(std::vector<Coordinates> &vec) {
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

auto count_antinodes(const Map &city_map) {
  const auto frequencies = get_frequencies(city_map);
  std::vector<Coordinates> antinodes {};
  for (const auto &frequency : frequencies) {
    const auto new_antinodes = get_antinodes_for_frequency(city_map, frequency);
    antinodes.insert(antinodes.end(), new_antinodes.begin(), new_antinodes.end());
  }
  remove_duplicates(antinodes);
  return antinodes.size();
}

void print_map(const std::vector<std::string> &vec) {
  for (const auto &row : vec) {
    std::cout << row << '\n';
  }
}

int main() {
  auto file = open_file(filename);
  auto city_map = file_to_string(file);
  std::cout << "Antinodes num: " << count_antinodes(city_map) << '\n';  
  return 0;
}
