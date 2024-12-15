#pragma once
#include "coordinates.hpp"
#include <string>
#include <unordered_map>

namespace std {
template <> struct hash<Coordinates<long>> {
  std::size_t operator()(const Coordinates<long> &coordinates) const {
    return std::hash<long>()(coordinates.x) ^ (std::hash<long>()(coordinates.y) << 1);
  }
};
} // namespace std

std::pair<std::unordered_map<Coordinates<long>, char>, std::string>
get_map_and_movement_from_file(const std::string &filename, const bool part_2);

void print_map(const std::unordered_map<Coordinates<long>, char> &map);
