#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <array>
#include <cmath>

static constexpr std::string_view filename = "data.txt";

auto open_file(const std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_line(std::ifstream &file) {
  std::string line_as_string;
  std::vector<size_t> line_as_vector_of_numbers;
  std::getline(file, line_as_string);
  std::istringstream iss(line_as_string);
  size_t number;
  while (iss >> number) {
    line_as_vector_of_numbers.push_back(number);
  }
  return line_as_vector_of_numbers;
}

constexpr auto rule_1(const size_t stone_number) -> size_t {
  if (stone_number != 0) {
    throw std::runtime_error("The first rule applied incorrectly");
  }
  return 1;
}

constexpr auto get_num_of_digits(size_t number) -> size_t {
  size_t digits = 0;
  while (number > 0) {
    number /= 10;
    ++digits;
  }
  return digits;  
}

constexpr auto has_even_num_of_digits(const size_t number) -> bool {
  return get_num_of_digits(number) % 2 == 0;
}

constexpr auto split_number_digits(const size_t number) -> std::array<size_t, 2> {
  const size_t number_of_digits = get_num_of_digits(number);
  const size_t first_number = number / std::pow(10, number_of_digits / 2);
  const size_t second_number = number % static_cast<size_t>(std::pow(10, number_of_digits / 2));
  return {first_number, second_number};
}

constexpr auto rule_2(const size_t stone_number) {
  if (!has_even_num_of_digits(stone_number)) {
    throw std::runtime_error("The second rule applied incorrectly");
  }
  return split_number_digits(stone_number);
}

constexpr auto rule_3(const size_t stone_number) {
  return stone_number * 2024;
}

auto blink(const std::vector<size_t> &line) {
  std::vector<size_t> new_line {};
  for (const auto number : line) {
    if (number == 0) {
      new_line.push_back(rule_1(number));
      continue;
    }
    if (has_even_num_of_digits(number)) {
      const auto [first_number, second_number] = rule_2(number);
      new_line.insert(new_line.end(), {first_number, second_number});
      continue;
    }
    new_line.push_back(rule_3(number));
  }
  return new_line;
}

auto split_vector_into_chunks(const std::vector<size_t> &line, const size_t chunk_size) {
  std::vector<std::vector<size_t>> chunks;
  for (size_t i = 0; i < line.size(); i += chunk_size) {
    const auto begin = line.begin() + i;
    const auto potential_end_of_chunk_index = line.begin() + i + chunk_size;
    const auto end = std::min(potential_end_of_chunk_index, line.end());
    chunks.push_back(std::vector<size_t>(begin, end));
  }
  return chunks;
}

auto blink_n_times(const std::vector<size_t> &line, const size_t n) -> size_t;

auto solve_in_chunks(const std::vector<size_t> &line, const size_t chunk_size, const size_t n) -> size_t {
  if (n == 0) {
    return line.size();
  }
  size_t num_of_stones = 0;
  const auto chunks = split_vector_into_chunks(line, chunk_size);
  for (const auto &chunk : chunks) {
    num_of_stones += blink_n_times(chunk, n);
  }
  return num_of_stones;
}

auto blink_n_times(const std::vector<size_t> &line, const size_t n) -> size_t {
  static constexpr size_t max_vector_size = 1'000'000;
  std::vector<size_t> new_line = line;
  for (size_t i = 0; i < n; ++i) {
    new_line = blink(new_line);
    if (new_line.size() > max_vector_size) {
      return solve_in_chunks(new_line, max_vector_size, n - i - 1);
    }
  }
  return new_line.size();
}

int main() {
  auto file = open_file(filename);
  const auto line_of_stones = file_to_line(file);
  std::cout << blink_n_times(line_of_stones, 25) << '\n'; // 199753
  std::cout << blink_n_times(line_of_stones, 75) << '\n';
  return 0;
}
