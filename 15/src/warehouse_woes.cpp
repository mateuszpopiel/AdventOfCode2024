#include "warehouse_woes.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Coordinates {
  ull x;
  ull y;

  bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }
};

namespace std {
template <> struct hash<Coordinates> {
  std::size_t operator()(const Coordinates &coordinates) const {
    return std::hash<ull>()(coordinates.x) ^ (std::hash<ull>()(coordinates.y) << 1);
  }
};
} // namespace std

void fill_map_with_line(std::unordered_map<Coordinates, char> &map, const std::string &line, const ull line_nr) {
  for (ull i = 0; i < line.size(); ++i) {
    map[{line_nr, i}] = line[i];
  }
}

std::pair<std::unordered_map<Coordinates, char>, std::string>
get_map_and_movement_from_file(const std::string &filename) {
  std::unordered_map<Coordinates, char> map{};
  std::string movement{};
  std::string line{};
  auto file = open_file(filename);
  for (ull i = 0; std::getline(file, line); ++i) {
    if (line.empty()) {
      continue;
    }
    if (line[0] == '#') {
      fill_map_with_line(map, line, i);
      continue;
    }
    movement += line;
  }
  if (file.is_open()) {
    file.close();
  }
  return {map, movement};
}

Coordinates get_next_position(const Coordinates &current_position, const char direction) {
  Coordinates next_position{current_position};
  switch (direction) {
  case 'v':
    ++next_position.x;
    break;
  case '^':
    --next_position.x;
    break;
  case '>':
    ++next_position.y;
    break;
  case '<':
    --next_position.y;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
  return next_position;
}

bool is_box_on_way(const Coordinates &robot, const char direction, const std::unordered_map<Coordinates, char> &map) {
  const auto next_position = get_next_position(robot, direction);
  if (map.find(next_position) == map.end()) {
    return false;
  }
  return map.at(next_position) == 'O';
}

void move_box(const Coordinates &box_to_move, const char direction, std::unordered_map<Coordinates, char> &map) {
  const auto next_box_pos = get_next_position(box_to_move, direction);
  if (map.find(next_box_pos) == map.end()) {
    return;
  }
  if (map.at(next_box_pos) == '#') {
    return;
  }
  if (map.at(next_box_pos) == 'O') {
    move_box(next_box_pos, direction, map);
  }
  if (map.at(next_box_pos) == '.') {
    map[next_box_pos] = 'O';
    map[box_to_move] = '.';
  }
}

Coordinates move_robot(const Coordinates &robot, const char direction, std::unordered_map<Coordinates, char> &map) {
  const auto next_position = get_next_position(robot, direction);
  if (map.find(next_position) == map.end()) {
    return robot;
  }
  if (map.at(next_position) == '#') {
    return robot;
  }
  if (is_box_on_way(robot, direction, map)) {
    move_box(next_position, direction, map);
  }
  if (is_box_on_way(robot, direction, map)) {
    return robot;
  }
  map[robot] = '.';
  map[next_position] = '@';
  return next_position;
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
    robot = move_robot(robot, direction, map);
  }
}

ull compute_sum_of_boxes_gps(const std::unordered_map<Coordinates, char> &map) {
  return std::accumulate(map.begin(), map.end(), 0u, [](const ull sum, const auto &box) {
    if (box.second != 'O') {
      return sum;
    }
    return sum + 100u * box.first.x + box.first.y;
  });
}

ull solve(const bool part_2) {
  auto [map, movement] = get_map_and_movement_from_file("data.txt");
  simulate_robot_movement(map, movement);
  if (!part_2) {
    return compute_sum_of_boxes_gps(map);
  }
  return 0;
}
