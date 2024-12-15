#include "warehouse_woes.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Coordinates {
  long x;
  long y;

  bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
  Coordinates operator+(const Coordinates &other) const { return {x + other.x, y + other.y}; }
};

namespace std {
template <> struct hash<Coordinates> {
  std::size_t operator()(const Coordinates &coordinates) const {
    return std::hash<long>()(coordinates.x) ^ (std::hash<long>()(coordinates.y) << 1);
  }
};
} // namespace std

static const std::unordered_map<char, Coordinates> movements{
    {'v', {1, 0}}, {'^', {-1, 0}}, {'>', {0, 1}}, {'<', {0, -1}}};

void print_map(const std::unordered_map<Coordinates, char> &map) {
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

void fill_map_with_line(std::unordered_map<Coordinates, char> &map, const std::string &line, const long line_nr) {
  for (auto i = 0u; i < line.size(); ++i) {
    map[{line_nr, i}] = line[i];
  }
}

void fill_map_with_line_part_2(std::unordered_map<Coordinates, char> &map, const std::string &line,
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

std::pair<std::unordered_map<Coordinates, char>, std::string>
get_map_and_movement_from_file(const std::string &filename, const bool part_2) {
  std::unordered_map<Coordinates, char> map{};
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

Coordinates move(const Coordinates &pos, const char direction, std::unordered_map<Coordinates, char> &map) {
  auto next_pos = pos + movements.at(direction);
  if (map.at(next_pos) == '#') {
    return pos;
  }
  if (map.at(next_pos) == 'O') {
    move(next_pos, direction, map);
  }
  if (map.at(next_pos) == '.') {
    map.at(next_pos) = map.at(pos);
    map.at(pos) = '.';
    return next_pos;
  }
  return pos;
}

Coordinates find_robot(const std::unordered_map<Coordinates, char> &map) {
  const auto robot_iter = std::find_if(map.begin(), map.end(), [](const auto &box) { return box.second == '@'; });
  if (robot_iter == map.end()) {
    throw std::runtime_error("Robot not found");
  }
  return robot_iter->first;
}

void simulate_robot_movement(std::unordered_map<Coordinates, char> &map, const std::string &movement) {
  auto robot = find_robot(map);
  for (const auto &direction : movement) {
    robot = move(robot, direction, map);
  }
}

long compute_sum_of_boxes_gps(const std::unordered_map<Coordinates, char> &map, const bool part_2) {
  const auto box_char = part_2 ? '[' : 'O';
  return std::accumulate(map.begin(), map.end(), 0u, [box_char](const long sum, const auto &box) {
    if (box.second != box_char) {
      return sum;
    }
    return sum + 100u * box.first.x + box.first.y;
  });
}

ull solve(const bool part_2) {
  auto [map, movement] = get_map_and_movement_from_file("data.txt", part_2);
  simulate_robot_movement(map, movement);
  return static_cast<ull>(compute_sum_of_boxes_gps(map, part_2));
}
