#pragma once
#include "coordinates.hpp"
#include <string>
#include <unordered_map>

std::pair<std::unordered_map<Coordinates<long>, char>, std::string>
get_map_and_movement_from_file(const std::string &filename, const bool part_2);

void print_map(const std::unordered_map<Coordinates<long>, char> &map);
