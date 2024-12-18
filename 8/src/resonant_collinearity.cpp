#include "resonant_collinearity.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using Map = std::vector<std::string>;
using Coordinates = std::pair<long long, long long>;

auto get_frequencies(const Map &city_map) {
  std::string frequencies;
  for (const auto &line : city_map) {
    for (const auto &c : line) {
      if ((std::isdigit(c) != 0 || std::isalpha(c) != 0) && (frequencies.find(c) == std::string::npos)) {
        frequencies.push_back(c);
      }
    }
  }
  return frequencies;
}

auto get_same_frequency_antennas_coordinates(const Map &city_map, const char frequency) {
  std::vector<Coordinates> antennas;
  for (size_t i = 0; i < city_map.size(); ++i) {
    for (size_t j = 0; j < city_map[i].size(); ++j) {
      if (city_map[i][j] == frequency) {
        antennas.emplace_back(i, j);
      }
    }
  }
  return antennas;
}

auto compute_vector(const Coordinates &a, const Coordinates &b) {
  return std::make_pair(b.first - a.first, b.second - a.second);
}

auto is_antinode_valid(const Coordinates &antinode, const Map &city_map) {
  return antinode.first >= 0 && static_cast<size_t>(antinode.first) < city_map.size() && antinode.second >= 0 &&
         static_cast<size_t>(antinode.second) < city_map[0].size();
}

auto get_next_antinode(const Coordinates &antenna, const Coordinates &harmonic_vector,
                       const Map &city_map) -> std::optional<Coordinates> {
  const auto potential_antinode =
      std::make_pair(antenna.first + harmonic_vector.first, antenna.second + harmonic_vector.second);
  if (is_antinode_valid(potential_antinode, city_map)) {
    return potential_antinode;
  }
  return std::nullopt;
}

auto get_next_antinode_coordinates(const Coordinates &antenna, const Coordinates &harmonic_vector) {
  return std::make_pair(antenna.first + harmonic_vector.first, antenna.second + harmonic_vector.second);
}

auto get_next_harmonic_antinodes(const Coordinates &antenna, const Coordinates &harmonic_vector, const Map &city_map) {
  std::vector<Coordinates> antinodes;
  auto antinode = get_next_antinode(antenna, harmonic_vector, city_map);
  for (auto next_antinode_coordiantes = get_next_antinode_coordinates(antenna, harmonic_vector);
       is_antinode_valid(next_antinode_coordiantes, city_map);
       next_antinode_coordiantes = get_next_antinode_coordinates(next_antinode_coordiantes, harmonic_vector)) {
    if (antinode.has_value()) {
      antinodes.push_back(antinode.value());
    }
    antinode = get_next_antinode(next_antinode_coordiantes, harmonic_vector, city_map);
  }
  return antinodes;
}

auto get_antinodes_for_two_antennas(const Coordinates &antenna1, const Coordinates &antenna2, const Map &city_map,
                                    const bool is_part_2) {
  std::vector<Coordinates> antinodes;
  const auto positive_vector_coordinates = compute_vector(antenna1, antenna2);
  const auto negative_vector_coordinates = compute_vector(antenna2, antenna1);
  if (!is_part_2) {
    const auto positive_antinode = get_next_antinode(antenna2, positive_vector_coordinates, city_map);
    const auto negative_antinode = get_next_antinode(antenna1, negative_vector_coordinates, city_map);
    if (positive_antinode.has_value()) {
      antinodes.push_back(positive_antinode.value());
    }
    if (negative_antinode.has_value()) {
      antinodes.push_back(negative_antinode.value());
    }
    return antinodes;
  }
  // Needs to insert antennas as well
  const auto positive_antinodes = get_next_harmonic_antinodes(antenna1, positive_vector_coordinates, city_map);
  const auto negative_antinodes = get_next_harmonic_antinodes(antenna1, negative_vector_coordinates, city_map);
  antinodes.insert(antinodes.end(), positive_antinodes.begin(), positive_antinodes.end());
  antinodes.insert(antinodes.end(), negative_antinodes.begin(), negative_antinodes.end());
  antinodes.push_back(antenna1);
  return antinodes;
}

auto get_antinodes_for_frequency(const Map &city_map, const char frequency, const bool is_part_2) {
  const auto antennas = get_same_frequency_antennas_coordinates(city_map, frequency);
  std::vector<Coordinates> antinodes;
  for (auto antenna1_iter = antennas.begin(); antenna1_iter != std::prev(antennas.end()); ++antenna1_iter) {
    for (auto antenna2_iter = std::next(antenna1_iter); antenna2_iter != antennas.end(); ++antenna2_iter) {
      const auto new_antinodes = get_antinodes_for_two_antennas(*antenna1_iter, *antenna2_iter, city_map, is_part_2);
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

auto count_antinodes(const Map &city_map, const bool is_part_2) {
  const auto frequencies = get_frequencies(city_map);
  std::vector<Coordinates> antinodes{};
  for (const auto &frequency : frequencies) {
    const auto new_antinodes = get_antinodes_for_frequency(city_map, frequency, is_part_2);
    antinodes.insert(antinodes.end(), new_antinodes.begin(), new_antinodes.end());
  }
  remove_duplicates(antinodes);
  return antinodes.size();
}

ull solve(const bool part_2) {
  const auto city_map = file_to_string("data.txt");
  return count_antinodes(city_map, part_2);
}
