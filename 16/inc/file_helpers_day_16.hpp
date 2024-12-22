#pragma once
#include "coordinates.hpp"
#include <string>
#include <unordered_map>

using Map = std::unordered_map<Coordinates<long>, char>;

Map get_map(const std::string &filename);

void print_map(const Map &map);
