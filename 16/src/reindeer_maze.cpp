#include "reindeer_maze.hpp"
#include "file_helpers_day_16.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <stack>
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

std::pair<std::vector<std::vector<Reindeer>>, ull> get_all_paths(const std::unordered_map<Tile, ull> &map,
                                                                 const Reindeer &reindeer, const Tile &end) {
  std::vector<std::vector<Reindeer>> all_shortest_paths;
  std::priority_queue<Node, std::vector<Node>, std::greater<>> pq;
  std::unordered_map<Reindeer, ull> visited;
  std::unordered_map<Reindeer, std::vector<Reindeer>> paths;

  // Initialize the priority queue
  pq.push({reindeer.position, reindeer.orientation, 0});
  visited[reindeer] = 0;
  paths[reindeer] = {reindeer};

  auto min_cost = std::numeric_limits<ull>::max();

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();

    auto current_cost = current.cost;
    Reindeer current_reindeer{current.position, current.orientation};

    if (current_cost > min_cost) {
      break;
    }

    if (current.position == end) {
      if (current_cost < min_cost) {
        all_shortest_paths.clear();
        min_cost = current_cost;
      }
      all_shortest_paths.push_back(paths[current_reindeer]);
      continue;
    }

    for (const char &dir : orientations) {
      const auto cost = (dir == current.orientation) ? move_cost : turn_cost + move_cost;
      const auto new_position = current.position + moves.at(dir);
      auto new_cost = current_cost + cost;

      if (map.find(new_position) == map.end()) {
        continue;
      }

      Reindeer neighbor_reindeer{new_position, dir};
      if (new_cost <= visited[neighbor_reindeer] || visited[neighbor_reindeer] == 0) {
        visited[neighbor_reindeer] = new_cost;
        pq.push({new_position, dir, new_cost});

        paths[neighbor_reindeer] = paths[current_reindeer];
        paths[neighbor_reindeer].push_back(neighbor_reindeer);
      }
    }
  }

  return {all_shortest_paths, min_cost};
}

std::vector<Tile> remove_duplicates(const std::vector<Tile> &tiles) {
  std::vector<Tile> unique_tiles = tiles;
  std::sort(unique_tiles.begin(), unique_tiles.end());
  unique_tiles.erase(std::unique(unique_tiles.begin(), unique_tiles.end()), unique_tiles.end());
  return unique_tiles;
}

std::vector<Tile> get_all_tiles_on_optimal_paths(const std::vector<std::vector<Reindeer>> &all_paths) {
  std::vector<Tile> all_tiles;
  for (const auto &path : all_paths) {
    for (const auto &reindeer : path) {
      all_tiles.push_back(reindeer.position);
    }
  }
  all_tiles = remove_duplicates(all_tiles);
  return all_tiles;
}

ull solve(const bool part_2) {
  const auto map = get_map("/home/mateusz/workspace/AdventOfCode2024/16/resources/data.txt");
  const auto filtered_map = get_filtered_map_with_costs(map);
  const Reindeer reindeer{find(map, 'S'), '>'};
  const auto [all_paths, lowest_cost] = get_all_paths(filtered_map, reindeer, find(map, 'E'));
  if (!part_2) {
    return lowest_cost;
  }
  return get_all_tiles_on_optimal_paths(all_paths).size();
}
