#include "plutionian_pebbles.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

auto parse_file_line(std::ifstream &file) -> std::unordered_map<uint64_t, uint64_t> {
  std::string line_as_string;
  std::unordered_map<uint64_t, uint64_t> line_as_map;
  std::getline(file, line_as_string);
  std::istringstream iss(line_as_string);
  auto number = 0ull;
  while (iss >> number) {
    line_as_map[number]++;
  }
  return line_as_map;
}

constexpr auto get_num_of_digits(uint64_t number) -> uint64_t {
  return number > 0 ? static_cast<uint64_t>(std::log10(number)) + 1 : 1;
}

constexpr auto split_number_digits(const uint64_t number) -> std::array<uint64_t, 2> {
  const auto number_of_digits = get_num_of_digits(number);
  const auto divisor = static_cast<uint64_t>(std::pow(10, number_of_digits / 2));
  const auto first_number = number / divisor;
  const auto second_number = number % divisor;
  return {first_number, second_number};
}

auto blink(const std::unordered_map<uint64_t, uint64_t> &line) -> std::unordered_map<uint64_t, uint64_t> {
  std::unordered_map<uint64_t, uint64_t> new_line;
  for (const auto &[stone, count] : line) {
    if (stone == 0) {
      new_line[1] += count;
      continue;
    }
    if (get_num_of_digits(stone) % 2 == 0) {
      const auto [first_part, second_part] = split_number_digits(stone);
      new_line[first_part] += count;
      new_line[second_part] += count;
      continue;
    }
    new_line[stone * 2024] += count;
  }
  return new_line;
}

auto blink_n_times(const std::unordered_map<uint64_t, uint64_t> &line, const uint64_t n) -> uint64_t {
  auto new_line = line;
  for (auto i = 0ull; i < n; ++i) {
    new_line = blink(new_line);
  }

  return std::accumulate(new_line.begin(), new_line.end(), 0ull,
                         [](const auto &acc, const auto &pair) { return acc + pair.second; });
}

auto get_line_of_stones(const std::string &filename) -> std::unordered_map<uint64_t, uint64_t> {
  auto file = open_file(filename);
  auto line_of_stones = parse_file_line(file);
  file.close();
  return line_of_stones;
}

ull solve(const bool part_2) {
  const auto line_of_stones = get_line_of_stones("data.txt");
  const auto N = part_2 ? 75u : 25u;
  return blink_n_times(line_of_stones, N);
}
