#include "monkey_market.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace std {
template <typename T, std::size_t N> struct hash<std::array<T, N>> {
  std::size_t operator()(const std::array<T, N> &arr) const noexcept {
    std::size_t seed = 0;
    for (const auto &elem : arr) {
      seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2); // Combine hashes
    }
    return seed;
  }
};
} // namespace std

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

uint64_t get_next_number(uint64_t num) {
  num ^= num << 6 & 0xffffff;
  num ^= num >> 5 & 0xffffff;
  return num ^ ((num << 11) & 0xffffff);
}

constexpr std::vector<uint64_t> generate_n_numbers(const uint64_t initial, const uint16_t n) {
  std::vector<uint64_t> numbers{initial};
  numbers.reserve(n + 1);
  for (uint16_t i = 1; i <= n; i++) {
    numbers.push_back(get_next_number(numbers.at(static_cast<uint16_t>(i - 1))));
  }
  return numbers;
}

std::vector<uint8_t> get_prices(const uint64_t initial_number, const uint16_t n) {
  auto secret_numbers = generate_n_numbers(initial_number, n);
  std::vector<uint8_t> prices(n, 0u);
  std::transform(secret_numbers.begin(), secret_numbers.end(), prices.begin(),
                 [](const auto num) { return static_cast<uint8_t>(num % 10); });
  return prices;
}

std::unordered_map<std::array<int8_t, 4>, uint64_t> get_map_of_sequences(const std::vector<uint64_t> &init_numbers,
                                                                         const uint16_t numbers_to_generate) {
  std::unordered_map<std::array<int8_t, 4>, uint64_t> sums;
  for (const auto &init_number : init_numbers) {
    std::unordered_set<std::array<int8_t, 4>> seen_sequences;
    const auto prices = get_prices(init_number, numbers_to_generate);
    for (uint16_t i = 4; i < static_cast<uint16_t>(prices.size() - 5); i++) {
      const std::array<int8_t, 4> diff_array = {
          static_cast<int8_t>(prices[i + 1] - prices[i]), static_cast<int8_t>(prices[i + 2] - prices[i + 1]),
          static_cast<int8_t>(prices[i + 3] - prices[i + 2]), static_cast<int8_t>(prices[i + 4] - prices[i + 3])};
      if (!seen_sequences.contains(diff_array)) {
        sums[diff_array] += prices[i + 4];
        seen_sequences.insert(diff_array);
      }
    }
  }
  return sums;
}

uint64_t get_best_bananas_amount(const std::vector<uint64_t> &init_numbers, const uint16_t numbers_to_generate) {
  const auto sums_map = get_map_of_sequences(init_numbers, numbers_to_generate);

  return std::max_element(sums_map.begin(), sums_map.end(),
                          [](const auto &a, const auto &b) { return a.second < b.second; })
      ->second;
}

uint64_t solve(const bool part_2) {
  const auto numbers = get_data("data.txt");
  if (!part_2) {
    return std::accumulate(numbers.begin(), numbers.end(), 0ull,
                           [](auto sum, const auto number) { return sum += generate_n_numbers(number, 2000).back(); });
  }
  return get_best_bananas_amount(numbers, 2000);
}
