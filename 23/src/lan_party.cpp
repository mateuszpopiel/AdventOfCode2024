#include "lan_party.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
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

std::set<std::set<std::string>> get_computer_pairs(const std::string &filename) {
  auto file = open_file(filename);
  std::set<std::set<std::string>> computer_pairs;
  std::string line;
  while (std::getline(file, line)) {
    std::string first = line.substr(0, 2);
    std::string second = line.substr(3, 2);
    const std::set<std::string> pcs{first, second};
    computer_pairs.insert(pcs);
  }
  file.close();
  return computer_pairs;
}

std::unordered_map<std::string, std::set<std::string>>
get_map_of_connections(const std::set<std::set<std::string>> &connections) {
  std::unordered_map<std::string, std::set<std::string>> map_of_connections;
  for (const auto &connection : connections) {
    const auto first = *connection.begin();
    const auto second = *std::next(connection.begin());
    map_of_connections[first].insert(second);
    map_of_connections[second].insert(first);
  }
  return map_of_connections;
}

bool one_of_pcs_starts_with_t(std::set<std::string> &connection) {
  return std::any_of(connection.begin(), connection.end(), [](const auto &pc) { return pc[0] == 't'; });
}

std::vector<std::string> get_common_pcs(const std::set<std::string> &first, const std::set<std::string> &second) {
  std::vector<std::string> commonc_pcs;
  for (const auto &pc : first) {
    if (second.contains(pc)) {
      commonc_pcs.push_back(pc);
    }
  }
  return commonc_pcs;
}

std::set<std::set<std::string>> get_connected_computers(const std::set<std::set<std::string>> &connected_pairs) {
  const auto map_of_connections = get_map_of_connections(connected_pairs);
  std::set<std::set<std::string>> sets_of_connected_cps;
  for (const auto &connection : connected_pairs) {
    const auto first_connections = map_of_connections.at(*connection.begin());
    const auto second_connections = map_of_connections.at(*std::next(connection.begin()));
    const auto common_pcs = get_common_pcs(first_connections, second_connections);
    for (const auto &pc : common_pcs) {
      auto new_connection = connection;
      new_connection.insert(pc);
      if (one_of_pcs_starts_with_t(new_connection)) {
        sets_of_connected_cps.insert(new_connection);
      }
    }
  }
  return sets_of_connected_cps;
}

uint64_t solve(const bool /*part_2*/) {
  const auto computer_pairs = get_computer_pairs("/home/mateusz/workspace/AdventOfCode2024/23/resources/data.txt");
  const auto connected_computers = get_connected_computers(computer_pairs);
  return connected_computers.size();
}
