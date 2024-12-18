#include "ram_run.hpp"
#include "coordinates.hpp"
#include "file_helpers.hpp"
#include <cstdint>
#include <fstream>
#include <functional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

static constexpr uint8_t map_size = 71;
static constexpr uint16_t bytes_to_simulate = 1024;

static const std::unordered_map<char, Coordinates<int8_t>> directions = {
    {'^', {0, -1}},
    {'>', {1, 0}},
    {'v', {0, 1}},
    {'<', {-1, 0}},
};

struct Node {
  ull steps;
  Coordinates<int8_t> position;

  bool operator>(const Node &other) const { return steps > other.steps; }
};

std::unordered_set<Coordinates<int8_t>> get_data(const std::string &filename) {
  auto file_str = get_input_from_multiline_file(filename);
  std::unordered_set<Coordinates<int8_t>> result;
  for (auto &line : file_str) {
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream iss(line);
    int16_t x = 0;
    int16_t y = 0;
    iss >> x >> y;
    result.insert({static_cast<int8_t>(x), static_cast<int8_t>(y)});
    if (result.size() == bytes_to_simulate) {
      break;
    }
  }
  return result;
}

bool is_next_position_valid(const std::unordered_set<Coordinates<int8_t>> &byte_positions,
                            const Coordinates<int8_t> &current, const char direction) {
  const auto next_position = current + directions.at(direction);
  return (next_position.x >= 0) && (next_position.x < map_size) && (next_position.y >= 0) &&
         (next_position.y < map_size) && (byte_positions.find(next_position) == byte_positions.end());
}

ull find_shortest_path(const std::unordered_set<Coordinates<int8_t>> &byte_positions) {
  const Coordinates<int8_t> start = {0, 0};
  const Coordinates<int8_t> end = {map_size - 1, map_size - 1};
  std::priority_queue<Node, std::vector<Node>, std::greater<>> queue;
  std::unordered_map<Coordinates<int8_t>, ull> visited;

  queue.push({0, start});
  visited[start] = 0;

  while (!queue.empty()) {
    const auto current = queue.top();
    queue.pop();

    if (current.position == end) {
      return current.steps;
    }

    for (const auto &[direction, offset] : directions) {
      const auto next_position = current.position + offset;
      const auto next_steps = current.steps + 1;

      if (!is_next_position_valid(byte_positions, current.position, direction)) {
        continue;
      }

      if (visited.contains(next_position) && (visited[next_position] <= next_steps)) {
        continue;
      }
      visited[next_position] = next_steps;
      queue.push({next_steps, next_position});
    }
  }
  return 0;
}

ull solve(const bool /*part_2*/) {
  const auto byte_positions = get_data("/home/mateusz/workspace/AdventOfCode2024/18/resources/data.txt");
  return find_shortest_path(byte_positions);
}
