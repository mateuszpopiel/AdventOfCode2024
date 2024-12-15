#include "warehouse_woes.hpp"
#include "file_helpers_day_15.hpp"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

static const std::unordered_map<char, Coordinates<long>> movements{
    {'v', {1, 0}}, {'^', {-1, 0}}, {'>', {0, 1}}, {'<', {0, -1}}};

Coordinates<long> move(const Coordinates<long> &pos, const char direction,
                       std::unordered_map<Coordinates<long>, char> &map) {
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

Coordinates<long> find_robot(const std::unordered_map<Coordinates<long>, char> &map) {
  const auto robot_iter = std::find_if(map.begin(), map.end(), [](const auto &box) { return box.second == '@'; });
  if (robot_iter == map.end()) {
    throw std::runtime_error("Robot not found");
  }
  return robot_iter->first;
}

void simulate_robot_movement(std::unordered_map<Coordinates<long>, char> &map, const std::string &movement) {
  auto robot = find_robot(map);
  for (const auto &direction : movement) {
    robot = move(robot, direction, map);
  }
}

long compute_sum_of_boxes_gps(const std::unordered_map<Coordinates<long>, char> &map, const bool part_2) {
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
