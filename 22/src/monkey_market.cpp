#include "monkey_market.hpp"
#include "file_helpers.hpp"
#include <cstdint>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<uint64_t> get_data(const std::string &filename) {
  auto file = open_file(filename);
  std::vector<uint64_t> result;
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    uint64_t val = 0;
    iss >> val;
    result.push_back(val);
  }
  file.close();
  return result;
}

constexpr uint64_t mix_and_prune(const uint64_t old, const uint64_t secret_num) {
  return (old ^ secret_num) & 0xffffff;
}

constexpr uint64_t get_nth_number(const uint64_t old, const uint16_t n) {
  auto current = old;
  for (uint16_t i = 0; i < n; i++) {
    const auto num_1 = mix_and_prune(current, current << 6);
    const auto num_2 = mix_and_prune(num_1, num_1 >> 5);
    current = mix_and_prune(num_2, num_2 << 11);
  }
  return current;
}

uint64_t solve(const bool part_2) {
  const auto numbers = get_data("data.txt");
  if (!part_2) {
    return std::accumulate(numbers.begin(), numbers.end(), 0ull,
                           [](auto sum, const auto number) { return sum += get_nth_number(number, 2000); });
  }
  return 0;
}
