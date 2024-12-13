#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

static constexpr std::string_view filename{"data.txt"};

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
  switch (map[i][j]) {
  case '^':
    return i == 0;
  case 'v':
    return i == map.size() - 1;
  case '<':
    return j == 0;
  case '>':
    return j == map[i].size() - 1;
  default:
    std::unreachable();
  }
}

auto is_guard_facing_obstacle(const LabMap &map, const Position &guards_position) {
  const auto [i, j] = guards_position;
  switch (map[i][j]) {
  case '^':
    return map[i - 1][j] == '#' || map[i - 1][j] == 'O';
  case 'v':
    return map[i + 1][j] == '#' || map[i + 1][j] == 'O';
  case '<':
    return map[i][j - 1] == '#' || map[i][j - 1] == 'O';
  case '>':
    return map[i][j + 1] == '#' || map[i][j + 1] == 'O';
  default:
    std::unreachable();
  }
}

auto turn_right(const char guards_orientation) {
  switch (guards_orientation) {
  case '^':
    return '>';
  case 'v':
    return '<';
  case '<':
    return '^';
  case '>':
    return 'v';
  default:
    std::unreachable();
  }
}

auto move_forward(const Position &position, const char orientation) {
  const auto [i, j] = position;
  switch (orientation) {
  case '^':
    return std::make_pair(i - 1, j);
  case 'v':
    return std::make_pair(i + 1, j);
  case '<':
    return std::make_pair(i, j - 1);
  case '>':
    return std::make_pair(i, j + 1);
  default:
    std::unreachable();
  }
}

void print_map(const LabMap &map) {
  for (const auto &line : map) {
    std::cout << line << '\n';
  }
}

auto get_obstacle_position(const Position &pos, const char orientation) {
  switch (orientation) {
  case '^':
    return std::make_pair(pos.first - 1, pos.second);
  case 'v':
    return std::make_pair(pos.first + 1, pos.second);
  case '<':
    return std::make_pair(pos.first, pos.second - 1);
  case '>':
    return std::make_pair(pos.first, pos.second + 1);
  default:
    std::unreachable();
  }
}

bool is_loop_detected(const std::vector<std::pair<Position, char>> &path) {
  // Smallest path is 4 steps, smallest loop would be 8, detected on 9th step
  if (path.size() < 10)
    return false;

  auto current_state = path.back();
  auto first_visit_it = std::find(path.begin(), path.end() - 1, current_state);
  if (first_visit_it == path.end() - 1) {
    return false;
  }
  const auto loop_length = path.end() - first_visit_it;
  for (int i = 0; i < loop_length; ++i) {
    if (path[first_visit_it - path.begin() + i] != path[path.size() - loop_length + i]) {
      return false;
    }
  }
  return true;
}

auto could_add_obstacle(const Position &guard_pos, const LabMap &map) {
  auto pos = guard_pos;
  auto map_copy = map;
  const auto [new_obst_i, new_obst_y] = get_obstacle_position(pos, map_copy[pos.first][pos.second]);
  std::vector<std::pair<Position, char>> path;
  map_copy[new_obst_i][new_obst_y] = 'O';
  for (auto position = find_guards_position(map_copy); !is_leaving_map(map_copy, position);
       position = find_guards_position(map_copy)) {
    const auto [i, j] = position;
    path.push_back({position, map_copy[i][j]});
    if (is_guard_facing_obstacle(map_copy, position)) {
      map_copy[i][j] = turn_right(map_copy[i][j]);
      continue;
    }
    const auto next_pos = move_forward(position, map_copy[i][j]);
    map_copy[next_pos.first][next_pos.second] = map_copy[i][j];
    map_copy[i][j] = '+';
    if (is_loop_detected(path)) {
      // print_map(map_copy);
      return true;
    }
  }
  return false;
}

auto simulate_guards_movement(LabMap &map) {
  auto num_of_obstacles_to_add = 0;
  for (auto position = find_guards_position(map); !is_leaving_map(map, position);
       position = find_guards_position(map)) {
    const auto [i, j] = position;
    if (is_guard_facing_obstacle(map, position)) {
      map[i][j] = turn_right(map[i][j]);
      continue;
    }
    if (could_add_obstacle(position, map)) {
      ++num_of_obstacles_to_add;
    }
    const auto [next_i, next_j] = move_forward(position, map[i][j]);
    map[next_i][next_j] = map[i][j];
    map[i][j] = 'X';
  }
  return num_of_obstacles_to_add;
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
  const auto num_of_obstacles_to_add = simulate_guards_movement(lab_map);
  std::cout << get_distinct_positions_number(lab_map) << '\n';
  std::cout << num_of_obstacles_to_add << '\n';
  return 0;
}
