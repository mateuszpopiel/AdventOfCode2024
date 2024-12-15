#include "warehouse_woes.hpp"
#include "file_helpers_day_15.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

static const std::unordered_map<char, Coordinates<long>> movements{
    {'v', {1, 0}}, {'^', {-1, 0}}, {'>', {0, 1}}, {'<', {0, -1}}};

bool can_move(const std::vector<Coordinates<long>> &positions, const std::unordered_map<Coordinates<long>, char> &map,
              const char direction) {
  switch (direction) {
  case 'v':
    return std::all_of(positions.begin(), positions.end(),
                       [&map](const auto &pos) { return map.at(pos + movements.at('v')) == '.'; });
  case '^':
    return std::all_of(positions.begin(), positions.end(),
                       [&map](const auto &pos) { return map.at(pos + movements.at('^')) == '.'; });
  case '>':
    return map.at(Coordinates<long>{positions.front().x, std::max(positions.front().y, positions.back().y) + 1}) == '.';
  case '<':
    return map.at(Coordinates<long>{positions.front().x, std::min(positions.front().y, positions.back().y) - 1}) == '.';
  }
  return false;
}

void move_box(const std::vector<Coordinates<long>> &box, const char direction,
              std::unordered_map<Coordinates<long>, char> &map) {
  if (direction == '<') {
    std::for_each(box.begin(), box.end(), [&map, direction](const auto &pos) {
      map.at(pos + movements.at(direction)) = map.at(pos);
      map.at(pos) = '.';
    });
    return;
  }
  // In case of 'V' or '^' order doesn't really matter, it had to be inverted for '>'
  std::for_each(box.rbegin(), box.rend(), [&map, direction](const auto &pos) {
    map.at(pos + movements.at(direction)) = map.at(pos);
    map.at(pos) = '.';
  });
}

Coordinates<long> move(const Coordinates<long> &pos, const char direction,
                       std::unordered_map<Coordinates<long>, char> &map, const bool part_2) {
  auto next_pos = pos + movements.at(direction);
  if (map.at(next_pos) == '#') {
    return pos;
  }
  if (map.at(next_pos) == 'O' || map.at(next_pos) == '[' || map.at(next_pos) == ']') {
    move(next_pos, direction, map, part_2);
  }
  if (map.at(pos) == '@' && map.at(next_pos) == '.') {
    map.at(next_pos) = map.at(pos);
    map.at(pos) = '.';
    return next_pos;
  }
  std::vector<Coordinates<long>> box{};
  if (!part_2) {
    box.push_back(pos);
  } else {
    if (map.at(pos) == '[') {
      box.push_back(pos);
      box.push_back(pos + movements.at('>'));
    } else {
      box.push_back(pos + movements.at('<'));
      box.push_back(pos);
    }
  }
  if (can_move(box, map, direction)) {
    move_box(box, direction, map);
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

void simulate_robot_movement(std::unordered_map<Coordinates<long>, char> &map, const std::string &movement,
                             const bool part_2) {
  auto robot = find_robot(map);
  for (const auto &direction : movement) {
    robot = move(robot, direction, map, part_2);
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
  simulate_robot_movement(map, movement, part_2);
  return static_cast<ull>(compute_sum_of_boxes_gps(map, part_2));
}
