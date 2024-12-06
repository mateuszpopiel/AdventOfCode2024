#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <vector>
#include <sstream>
#include <array>
#include <string>
#include <utility>

static constexpr std::string_view filename {"data.txt"};

using LabMap = std::vector<std::string>;
using Position = std::pair<size_t, size_t>;

auto open_file(const std::string_view &filename = filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_string(std::ifstream &file) {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}

auto find_guards_position(const LabMap &map) {
  for (size_t i = 0; i < map.size(); ++i) {
    for (size_t j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == '^' || map[i][j] == 'v' || map[i][j] == '<' || map[i][j] == '>') {
        return std::make_pair(i, j);
      }
    }
  }
  throw std::runtime_error("Guard was not found");
}

auto is_leaving_map(const LabMap &map, const Position &guards_position) {
  const auto [i, j] = guards_position;
  switch (map[i][j])
  {
    case '^': return i == 0;
    case 'v': return i == map.size() - 1;
    case '<': return j == 0;
    case '>': return j == map[i].size() - 1;
    default: std::unreachable();
  }
}

auto is_guard_facing_obstacle(const LabMap &map, const Position &guards_position) {
  const auto [i, j] = guards_position;
  switch (map[i][j])
  {
    case '^': return map[i - 1][j] == '#';
    case 'v': return map[i + 1][j] == '#';
    case '<': return map[i][j - 1] == '#';
    case '>': return map[i][j + 1] == '#';
    default: std::unreachable();
  }
}

auto turn_right(const char guards_orientation) {
  switch (guards_orientation)
  {
    case '^': return '>';
    case 'v': return '<';
    case '<': return '^';
    case '>': return 'v';
    default: std::unreachable();
  }
}

auto move_forward(const Position &position, const char orientation) {
  const auto [i, j] = position;
  switch (orientation)
  {
    case '^': return std::make_pair(i - 1, j);
    case 'v': return std::make_pair(i + 1, j);
    case '<': return std::make_pair(i, j - 1);
    case '>': return std::make_pair(i, j + 1);
    default: std::unreachable();
  }
}

void print_map(const LabMap &map) {
  for (const auto &line : map) {
    std::cout << line << '\n';
  }
}

void simulate_guards_movement(LabMap &map) {
  for (auto position = find_guards_position(map);
       !is_leaving_map(map, position);
       position = find_guards_position(map)) {
    const auto [i, j] = position;
    if (is_guard_facing_obstacle(map, position)) {
      map[i][j] = turn_right(map[i][j]);
      continue;
    }
    const auto [next_i, next_j] = move_forward(position, map[i][j]);
    map[next_i][next_j] = map[i][j];
    map[i][j] = 'X';
  }
}

auto get_distinct_positions_number(const LabMap &filled_map) {
  size_t distinct_positions = 0;
  for (const auto &line : filled_map) {
    distinct_positions += std::count(line.begin(), line.end(), 'X');
  }
  return distinct_positions + 1; // +1 for the leaving position
}

int main() {
  auto file = open_file(filename);
  auto lab_map = file_to_string(file);
  simulate_guards_movement(lab_map);
  std::cout << get_distinct_positions_number(lab_map) << '\n';
  return 0;
}
