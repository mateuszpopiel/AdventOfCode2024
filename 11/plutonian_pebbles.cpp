#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

static constexpr std::string_view filename = "data.txt";

auto open_file(const std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto parse_file_line(std::ifstream &file) -> std::unordered_map<uint64_t, uint64_t> {
  std::string line_as_string;
  std::unordered_map<uint64_t, uint64_t> line_as_map;
  std::getline(file, line_as_string);
  std::istringstream iss(line_as_string);
  uint64_t number;
  while (iss >> number) {
    line_as_map[number]++;
  }
  return line_as_map;
}

constexpr auto get_num_of_digits(uint64_t number) -> uint64_t {
  return number > 0 ? static_cast<uint64_t>(std::log10(number)) + 1 : 1;
}

constexpr auto split_number_digits(const uint64_t number) -> std::array<uint64_t, 2> {
  const uint64_t number_of_digits = get_num_of_digits(number);
  const uint64_t divisor = std::pow(10, number_of_digits / 2);
  const uint64_t first_number = number / divisor;
  const uint64_t second_number = number % divisor;
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
  for (uint64_t i = 0; i < n; ++i) {
    new_line = blink(new_line);
  }

  uint64_t sum = 0;
  for (const auto &[_, count] : new_line) {
    sum += count;
  }
  return sum;
}

int main() {
  auto file = open_file(filename);
  auto line_of_stones = parse_file_line(file);

  std::cout << blink_n_times(line_of_stones, 25) << '\n'; // 199753
  std::cout << blink_n_times(line_of_stones, 75) << '\n'; // 239413123020116

  return 0;
}
