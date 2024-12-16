#include "reindeer_maze.hpp"
#include "file_helpers_day_16.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Tile = Coordinates<long>;
static constexpr std::array<char, 4> orientations{'^', '>', 'v', '<'};
static const std::unordered_map<char, Coordinates<long>> moves{
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};
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
  std::vector<Tile> path;
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

std::pair<std::vector<std::vector<Tile>>, ull> find_all_paths_bfs(const std::unordered_map<Tile, ull> &map,
                                                                  const Tile &start, const Tile &end) {
  std::vector<std::vector<Tile>> all_shortest_paths;
  std::queue<Node> q;
  std::unordered_map<Tile, ull> path_costs = map;

  q.push({start, '>', 0, {start}});
  path_costs[start] = 0;
  auto min_cost = std::numeric_limits<ull>::max();

  while (!q.empty()) {
    const auto current_node = q.front();
    q.pop();

    if (current_node.cost > min_cost) {
      continue;
    }

    if (current_node.position == end) {
      if (current_node.cost < min_cost) {
        min_cost = current_node.cost;
        all_shortest_paths = {current_node.path};
      } else if (current_node.cost == min_cost) {
        all_shortest_paths.push_back(current_node.path);
      }
      continue;
    }

    for (const auto &dir : orientations) {
      const auto new_cost = current_node.cost + (current_node.orientation == dir ? move_cost : turn_cost + move_cost);
      const auto new_position = current_node.position + moves.at(dir);

      if (map.find(new_position) == map.end()) {
        continue;
      }

      if (new_cost > path_costs[new_position]) {
        continue;
      }

      path_costs[new_position] = new_cost;
      auto new_path = current_node.path;
      new_path.push_back(new_position);
      q.push({new_position, dir, new_cost, new_path});
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

ull get_num_of_unique_tiles(const std::vector<std::vector<Tile>> &paths) {
  std::vector<Tile> tiles;
  for (const auto &path : paths) {
    for (const auto &tile : path) {
      tiles.push_back(tile);
    }
  }
  return remove_duplicates(tiles).size();
}

ull solve(const bool part_2) {
  const auto map = get_map("/home/mateusz/workspace/AdventOfCode2024/16/resources/data.txt");
  const auto filtered_map = get_filtered_map_with_costs(map);
  const auto [paths_1, lowest_cost] = find_all_paths_bfs(filtered_map, find(map, 'S'), find(map, 'E'));
  const auto [paths_2, _] = find_all_paths_bfs(filtered_map, find(map, 'E'), find(map, 'S'));
  if (!part_2) {
    return lowest_cost;
  }
  auto paths = paths_1;
  paths.insert(paths.end(), paths_2.begin(), paths_2.end());
  return get_num_of_unique_tiles(paths);
}