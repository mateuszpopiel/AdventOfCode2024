#include "reindeer_maze.hpp"
#include "file_helpers_day_16.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using Tile = Coordinates<long>;
static constexpr std::array<char, 4> orientations{'^', '>', 'v', '<'};
static const std::unordered_map<char, Coordinates<long>> moves{
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {-0, -1}}};

struct Reindeer {
  Tile position;
  char orientation = '>';
};

struct Node {
  Tile position;
  char orientation;
  ull cost;

  bool operator>(const Node &other) const { return cost > other.cost; }
};

std::unordered_map<Tile, ull> get_filtered_map_with_costs(const Map &map) {
  std::unordered_map<Tile, ull> filtered_map;
  for (const auto &[coordinates, tile] : map) {
    if (tile == 'S') {
      filtered_map[coordinates] = 0;
    } else if (tile != '#') {
      filtered_map[coordinates] = std::numeric_limits<ull>::max();
    }
  }
  return filtered_map;
}

Tile find_start(const Map &map) {
  const auto start_iter = std::find_if(map.begin(), map.end(), [](const auto &pair) { return pair.second == 'S'; });
  if (start_iter == map.end()) {
    throw std::runtime_error("No start found");
  }
  return start_iter->first;
}

Tile find_end(const Map &map) {
  const auto end_iter = std::find_if(map.begin(), map.end(), [](const auto &pair) { return pair.second == 'E'; });
  if (end_iter == map.end()) {
    throw std::runtime_error("No end found");
  }
  return end_iter->first;
}

std::pair<Tile, char> get_tile_and_orientation_on_the_right(const Reindeer &reindeer) {
  const auto *const reindeer_orientation_iter =
      std::find(orientations.begin(), orientations.end(), reindeer.orientation);
  if (reindeer_orientation_iter == std::prev(orientations.end())) {
    return {reindeer.position + moves.at(orientations.front()), orientations.front()};
  }
  return {reindeer.position + moves.at(*std::next(reindeer_orientation_iter)), *std::next(reindeer_orientation_iter)};
}

Tile get_tile_in_front(const Reindeer &reindeer) { return reindeer.position + moves.at(reindeer.orientation); }

std::pair<Tile, char> get_tile_and_orientation_on_the_left(const Reindeer &reindeer) {
  const auto *const reindeer_orientation_iter =
      std::find(orientations.begin(), orientations.end(), reindeer.orientation);
  if (reindeer_orientation_iter == orientations.begin()) {
    return {reindeer.position + moves.at(orientations.back()), orientations.back()};
  }
  return {reindeer.position + moves.at(*std::prev(reindeer_orientation_iter)), *std::prev(reindeer_orientation_iter)};
}

ull find_smallest_cost_path(const std::unordered_map<Tile, ull> &map, const Reindeer &reindeer, const Tile &end) {
  std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;
  std::unordered_map<Tile, std::unordered_map<char, ull, std::hash<char>>> visited;

  pq.push({reindeer.position, reindeer.orientation, 0});
  visited[reindeer.position][reindeer.orientation] = 0;

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();

    if (current.position == end) {
      return current.cost;
    }

    // Explore next states
    for (const char &dir : orientations) {
      const ull turn_cost = (dir == current.orientation) ? 0 : 1000;

      // New orientation and position
      const auto move = moves.at(dir);
      const Tile new_position = {current.position.x + move.x, current.position.y + move.y};
      ull new_cost = current.cost + turn_cost;

      // Check if the new position is valid and update cost
      if (map.find(new_position) != map.end()) {
        new_cost += 1; // Move forward cost
        if (visited[new_position][dir] == 0 || new_cost < visited[new_position][dir]) {
          visited[new_position][dir] = new_cost;
          pq.push({new_position, dir, new_cost});
        }
      }
    }
  }

  // If no path found
  return std::numeric_limits<ull>::max();
}

ull solve(const bool /*part_2*/) {
  const auto map = get_map("data.txt");
  const auto filtered_map = get_filtered_map_with_costs(map);
  const Reindeer reindeer{find_start(map), '>'};
  return find_smallest_cost_path(filtered_map, reindeer, find_end(map));
}
