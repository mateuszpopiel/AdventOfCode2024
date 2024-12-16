#include "reindeer_maze.hpp"
#include "file_helpers_day_16.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

using Tile = Coordinates<long>;
static constexpr std::array<char, 4> orientations{'^', '>', 'v', '<'};
static const std::unordered_map<char, Coordinates<long>> moves{
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {-0, -1}}};
static constexpr ull turn_cost = 1000;
static constexpr ull move_cost = 1;

struct Reindeer {
  Tile position;
  char orientation = '>';

  bool operator==(const Reindeer &other) const {
    return position == other.position && orientation == other.orientation;
  }
};

namespace std {
template <> struct hash<Reindeer> {
  std::size_t operator()(const Reindeer &r) const noexcept {
    return std::hash<long>()(r.position.x) ^ std::hash<long>()(r.position.y) ^ std::hash<char>()(r.orientation);
  }
};
} // namespace std

struct Node {
  Tile position;
  char orientation;
  ull cost;

  bool operator>(const Node &other) const { return cost > other.cost; }
};

std::unordered_map<Tile, ull> get_filtered_map_with_costs(const Map &map) {
  std::unordered_map<Tile, ull> filtered_map;
  for (const auto &[coordinates, tile] : map) {
    if (tile != '#') {
      filtered_map[coordinates] = std::numeric_limits<ull>::max();
    }
  }
  return filtered_map;
}

Tile find(const Map &map, const char to_find) {
  const auto start_iter =
      std::find_if(map.begin(), map.end(), [to_find](const auto &pair) { return pair.second == to_find; });
  if (start_iter == map.end()) {
    throw std::runtime_error("No start found");
  }
  return start_iter->first;
}

ull find_smallest_cost_path(const std::unordered_map<Tile, ull> &map, const Reindeer &reindeer, const Tile &end) {
  std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;
  std::unordered_map<Reindeer, ull> visited;

  pq.push({reindeer.position, reindeer.orientation, 0});
  visited[reindeer] = 0;

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();

    if (current.position == end) {
      return current.cost;
    }

    // Explore next states
    for (const char &dir : orientations) {
      const ull cost = (dir == current.orientation) ? 0 : turn_cost;

      // New orientation and position
      const auto move = moves.at(dir);
      const Tile new_position = current.position + move;
      ull new_cost = current.cost + cost;

      // Check if the new position is valid and update cost
      if (map.find(new_position) != map.end()) {
        new_cost += move_cost;
        if (visited[{new_position, dir}] == 0 || new_cost < visited[{new_position, dir}]) {
          visited[{new_position, dir}] = new_cost;
          pq.push({new_position, dir, new_cost});
        }
      }
    }
  }

  return std::numeric_limits<ull>::max();
}

ull solve(const bool part_2) {
  const auto map = get_map("data.txt");
  const auto filtered_map = get_filtered_map_with_costs(map);
  const Reindeer reindeer{find(map, 'S'), '>'};
  const auto lowest_cost = find_smallest_cost_path(filtered_map, reindeer, find(map, 'E'));
  if (!part_2) {
    return lowest_cost;
  }
  return 0;
}
