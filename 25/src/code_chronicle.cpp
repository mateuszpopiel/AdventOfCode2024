#include "code_chronicle.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace std {
template <typename T, std::size_t N> struct hash<std::array<T, N>> {
  std::size_t operator()(const std::array<T, N> &arr) const noexcept {
    std::size_t seed = 0;
    for (const auto &elem : arr) {
      seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
  }
};
} // namespace std

std::array<uint8_t, 5> get_lock_or_key(const std::vector<std::string> &lock_str) {
  std::array<uint8_t, 5> lock_or_key{};
  for (const auto &line : lock_str) {
    for (auto i = 0u; i < lock_or_key.size(); i++) {
      if (line[i] == '#') {
        // NOLINTNEXTLINE(*-constant-array-index)
        ++lock_or_key[i];
      }
    }
  }
  return lock_or_key;
}

std::pair<std::unordered_set<std::array<uint8_t, 5>>, std::unordered_set<std::array<uint8_t, 5>>>
get_locks_and_keys(const std::string &filename) {
  const auto file_str = file_to_string(filename);
  std::unordered_set<std::array<uint8_t, 5>> locks;
  std::unordered_set<std::array<uint8_t, 5>> keys;
  std::vector<std::string> lock_or_key_str;
  for (const auto &line : file_str) {
    if (line.empty()) {
      continue;
    }
    lock_or_key_str.push_back(line);
    if (lock_or_key_str.size() != 7) {
      continue;
    }
    const auto lock_or_key = get_lock_or_key(lock_or_key_str);
    if (lock_or_key_str.front().front() == '#') {
      locks.insert(lock_or_key);
    } else {
      keys.insert(lock_or_key);
    }
    lock_or_key_str.clear();
  }
  return std::make_pair(locks, keys);
}

uint64_t how_many_locks_and_keys_fit(const std::unordered_set<std::array<uint8_t, 5>> &locks,
                                     const std::unordered_set<std::array<uint8_t, 5>> &keys) {
  uint64_t matches = 0;
  for (const auto &lock : locks) {
    for (const auto &key : keys) {
      if (std::none_of(lock.begin(), lock.end(),
                       [&key, _key_id = 0u](const auto _lock) mutable { return (_lock + key.at(_key_id++) > 7); })) {
        matches++;
      }
    }
  }
  return matches;
}

uint64_t solve() {
  const auto [locks, keys] = get_locks_and_keys("data.txt");
  return how_many_locks_and_keys_fit(locks, keys);
}
