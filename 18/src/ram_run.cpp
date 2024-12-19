#include "ram_run.hpp"
#include "coordinates.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>

static constexpr uint8_t map_size = 71;

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

std::vector<Coordinates<int8_t>> get_data(const std::string &filename, const uint16_t bytes_to_simulate) {
  auto file_str = file_to_string(filename);
  std::vector<Coordinates<int8_t>> result;
  for (auto &line : file_str) {
    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream iss(line);
    int16_t x = 0;
    int16_t y = 0;
    iss >> x >> y;
    result.emplace_back(static_cast<int8_t>(x), static_cast<int8_t>(y));
    if (result.size() == bytes_to_simulate) {
      break;
    }
  }
  return result;
}

bool is_next_position_valid(const std::vector<Coordinates<int8_t>> &byte_positions, const Coordinates<int8_t> &current,
                            const char direction) {
  const auto next_position = current + directions.at(direction);
  return (next_position.x >= 0) && (next_position.x < map_size) && (next_position.y >= 0) &&
         (next_position.y < map_size) &&
         (std::find(byte_positions.begin(), byte_positions.end(), next_position) == byte_positions.end());
}

ull find_shortest_path(const std::vector<Coordinates<int8_t>> &byte_positions) {
  const Coordinates<int8_t> start = {0, 0};
  const Coordinates<int8_t> end = {map_size - 1, map_size - 1};
  std::priority_queue<Node, std::vector<Node>, std::greater<>> queue;
  std::unordered_map<Coordinates<int8_t>, ull> visited;

  queue.push({0, start});
  visited[start] = 0;
  Coordinates<int8_t> current_position;

  while (!queue.empty()) {
    const auto current = queue.top();
    current_position = current.position;
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
  return std::numeric_limits<ull>::max();
}

std::string find_breaking_byte(const std::vector<Coordinates<int8_t>> &byte_positions) {
  ull low = 0;
  ull high = byte_positions.size();
  Coordinates<int8_t> breaking_obstacle;

  while (low < high) {
    const auto mid = low + (high - low) / 2;

    const std::vector<Coordinates<int8_t>> subset(
        byte_positions.begin(),
        std::next(byte_positions.begin(), static_cast<std::vector<Coordinates<int8_t>>::difference_type>(mid)));
    const auto steps = find_shortest_path(subset);

    if (steps == std::numeric_limits<ull>::max()) {
      breaking_obstacle = byte_positions[mid - 1];
      high = mid;
    } else {
      low = mid + 1;
    }
  }

  return breaking_obstacle.to_string();
}

std::variant<ull, std::string> solve(const bool part_2) {
  const uint16_t bytes_to_simulate = part_2 ? std::numeric_limits<uint16_t>::max() : 1024;
  const auto byte_positions = get_data("data.txt", bytes_to_simulate);
  if (!part_2) {
    return find_shortest_path(byte_positions);
  }
  return find_breaking_byte(byte_positions);
}
