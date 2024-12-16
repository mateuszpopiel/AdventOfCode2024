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

using Map = std::unordered_map<Coordinates<long>, char>;

Map get_map(const std::string &filename);

void print_map(const Map &map);
