#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <numeric>
#include <cstdint>
#include <cmath>

static constexpr std::string_view filename = "data.txt";

auto open_file(const std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_line(std::ifstream &file) -> std::unordered_map<uint64_t, uint64_t> {
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
  uint64_t digits = 0;
  while (number > 0) {
    number /= 10;
    ++digits;
  }
  return digits;
}

constexpr auto split_number_digits(const uint64_t number) -> std::array<uint64_t, 2> {
  const uint64_t number_of_digits = get_num_of_digits(number);
  const uint64_t first_number = number / std::pow(10, number_of_digits / 2);
  const uint64_t second_number = number % static_cast<uint64_t>(std::pow(10, number_of_digits / 2));
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

  return std::accumulate(new_line.begin(), new_line.end(), 0, [](const uint64_t sum, const auto &pair) {
    return sum + pair.second;
  });
}

int main() {
  auto file = open_file(filename);
  auto line_of_stones = file_to_line(file);

  std::cout << blink_n_times(line_of_stones, 25) << '\n'; // 199753
  std::cout << blink_n_times(line_of_stones, 75) << '\n'; // 239413123020116
  return 0;
}
