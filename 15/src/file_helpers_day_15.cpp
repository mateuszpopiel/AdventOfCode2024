#include "file_helpers_day_15.hpp"
#include "file_helpers.hpp"
#include <iostream>
#include <string>

void fill_map_with_line(std::unordered_map<Coordinates<long>, char> &map, const std::string &line, const long line_nr) {
  for (auto i = 0u; i < line.size(); ++i) {
    map[{line_nr, i}] = line[i];
  }
}

void fill_map_with_line_part_2(std::unordered_map<Coordinates<long>, char> &map, const std::string &line,
                               const long line_nr) {
  for (auto i = 0u; i < line.size(); ++i) {
    switch (line[i]) {
    case '@':
      map[{line_nr, i * 2u}] = '@';
      map[{line_nr, i * 2u + 1u}] = '.';
      break;
    case 'O':
      map[{line_nr, i * 2u}] = '[';
      map[{line_nr, i * 2u + 1u}] = ']';
      break;
    case '#':
      map[{line_nr, i * 2u}] = '#';
      map[{line_nr, i * 2u + 1u}] = '#';
      break;
    case '.':
      map[{line_nr, i * 2u}] = '.';
      map[{line_nr, i * 2u + 1u}] = '.';
      break;
    }
  }
}

std::pair<std::unordered_map<Coordinates<long>, char>, std::string>
get_map_and_movement_from_file(const std::string &filename, const bool part_2) {
  std::unordered_map<Coordinates<long>, char> map{};
  std::string movement{};
  std::string line{};
  auto file = open_file(filename);
  for (long i = 0; std::getline(file, line); ++i) {
    if (line.empty()) {
      continue;
    }
    if (line[0] == '#') {
      part_2 ? fill_map_with_line_part_2(map, line, i) : fill_map_with_line(map, line, i);
      continue;
    }
    movement += line;
  }
  if (file.is_open()) {
    file.close();
  }
  return {map, movement};
}

void print_map(const std::unordered_map<Coordinates<long>, char> &map) {
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
