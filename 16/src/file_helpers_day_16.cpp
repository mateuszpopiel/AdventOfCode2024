#include "file_helpers_day_16.hpp"
#include "file_helpers.hpp"
#include <iostream>
#include <string>

void fill_map_with_line(Map &map, const std::string &line, const long line_nr) {
  for (auto i = 0u; i < line.size(); ++i) {
    map[{line_nr, i}] = line[i];
  }
}

Map get_map(const std::string &filename) {
  std::unordered_map<Coordinates<long>, char> map{};
  std::string line{};
  auto file = open_file(filename);
  for (long i = 0; std::getline(file, line); ++i) {
    fill_map_with_line(map, line, i);
  }
  if (file.is_open()) {
    file.close();
  }
  return map;
}

void print_map(const Map &map) {
  long max_x = 0;
  long max_y = 0;
  for (const auto &[coordinates, _] : map) {
    max_x = std::max(max_x, coordinates.x);
    max_y = std::max(max_y, coordinates.y);
  }
  for (long i = 0; i <= max_x; ++i) {
    for (long j = 0; j <= max_y; ++j) {
      if (map.find({i, j}) == map.end()) {
        std::cout << ' ';
        continue;
      }
      std::cout << map.at({i, j});
    }
    std::cout << '\n';
  }
}
