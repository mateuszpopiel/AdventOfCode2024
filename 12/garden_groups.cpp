#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

static constexpr std::string_view filename = "data.txt";

using Map = std::vector<std::string>;

auto open_file(const std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_string(std::ifstream &file) -> Map {
  std::string row;
  std::vector<std::string> file_as_string;
  while (std::getline(file, row)) {
    file_as_string.push_back(row);
  }
  return file_as_string;
}

struct Region {
  size_t area;
  size_t perimeter;

  Region operator+=(const Region &other) {
    area += other.area;
    perimeter += other.perimeter;
    return *this;
  }
};

struct Coordinates {
  size_t x;
  size_t y;

  bool operator==(const Coordinates &other) const { return x == other.x && y == other.y; }

  bool operator<(const Coordinates &other) const {
    if (x < other.x) {
      return true;
    }
    if (x == other.x) {
      return y < other.y;
    }
    return false;
  }

  bool operator>(const Coordinates &other) const { return !(*this < other) && !(*this == other); }
};

auto find_region_beginning(const Map &map) -> std::optional<Coordinates> {
  for (size_t x = 0; x < map.size(); ++x) {
    for (size_t y = 0; y < map[x].size(); ++y) {
      if (std::isalpha(map[x][y])) {
        return Coordinates{x, y};
      }
    }
  }
  return std::nullopt;
}

auto is_cell_on_the_right_in_region(const Map &map, const Coordinates current_cell, const char region_type) -> bool {
  return current_cell.y + 1 < map[current_cell.x].size() && map[current_cell.x][current_cell.y + 1] == region_type;
}

auto is_cell_below_in_region(const Map &map, const Coordinates current_cell, const char region_type) -> bool {
  return current_cell.x + 1 < map.size() && map[current_cell.x + 1][current_cell.y] == region_type;
}

auto is_cell_on_the_left_in_region(const Map &map, const Coordinates current_cell, const char region_type) -> bool {
  return current_cell.y != 0 && map[current_cell.x][current_cell.y - 1] == region_type;
}

auto is_cell_above_in_region(const Map &map, const Coordinates current_cell, const char region_type) -> bool {
  return current_cell.x != 0 && map[current_cell.x - 1][current_cell.y] == region_type;
}

auto is_cell_on_upper_left_diagonal_in_region(const Map &map, const Coordinates current_cell,
                                              const char region_type) -> bool {
  return current_cell.x != 0 && current_cell.y != 0 && map[current_cell.x - 1][current_cell.y - 1] == region_type;
}

auto is_cell_on_upper_right_diagonal_in_region(const Map &map, const Coordinates current_cell,
                                               const char region_type) -> bool {
  return current_cell.x != 0 && current_cell.y + 1 < map[current_cell.x].size() &&
         map[current_cell.x - 1][current_cell.y + 1] == region_type;
}

auto is_cell_on_lower_left_diagonal_in_region(const Map &map, const Coordinates current_cell,
                                              const char region_type) -> bool {
  return current_cell.x + 1 < map.size() && current_cell.y != 0 &&
         map[current_cell.x + 1][current_cell.y - 1] == region_type;
}

auto is_cell_on_lower_right_diagonal_in_region(const Map &map, const Coordinates current_cell,
                                               const char region_type) -> bool {
  return current_cell.x + 1 < map.size() && current_cell.y + 1 < map[current_cell.x].size() &&
         map[current_cell.x + 1][current_cell.y + 1] == region_type;
}

auto was_cell_checked(const std::vector<Coordinates> &checked_cells, const Coordinates cell) -> bool {
  return std::any_of(checked_cells.begin(), checked_cells.end(),
                     [cell](const Coordinates &checked_cell) { return checked_cell == cell; });
}

auto get_cell_perimeter(const Map &map, const Coordinates current_cell, const char region_type,
                        std::vector<Coordinates> &checked_cells) -> size_t {
  size_t perimeter = 0;

  if (!is_cell_above_in_region(map, current_cell, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{current_cell.x - 1, current_cell.y})) {
    ++perimeter;
  }
  if (!is_cell_on_the_left_in_region(map, current_cell, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{current_cell.x, current_cell.y - 1})) {
    ++perimeter;
  }
  if (!is_cell_below_in_region(map, current_cell, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{current_cell.x + 1, current_cell.y})) {
    ++perimeter;
  }
  if (!is_cell_on_the_right_in_region(map, current_cell, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{current_cell.x, current_cell.y + 1})) {
    ++perimeter;
  }
  return perimeter;
}

auto get_region(Map &map, const Coordinates region_beginning, std::vector<Coordinates> &checked_cells) -> Region {
  const auto region_type = map[region_beginning.x][region_beginning.y];
  Region region{0, 0};
  checked_cells.push_back(region_beginning);
  if (is_cell_on_the_right_in_region(map, region_beginning, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{region_beginning.x, region_beginning.y + 1})) {
    region += get_region(map, {region_beginning.x, region_beginning.y + 1}, checked_cells);
  }
  if (is_cell_below_in_region(map, region_beginning, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{region_beginning.x + 1, region_beginning.y})) {
    region += get_region(map, {region_beginning.x + 1, region_beginning.y}, checked_cells);
  }
  if (is_cell_on_the_left_in_region(map, region_beginning, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{region_beginning.x, region_beginning.y - 1})) {
    region += get_region(map, {region_beginning.x, region_beginning.y - 1}, checked_cells);
  }
  if (is_cell_above_in_region(map, region_beginning, region_type) &&
      !was_cell_checked(checked_cells, Coordinates{region_beginning.x - 1, region_beginning.y})) {
    region += get_region(map, {region_beginning.x - 1, region_beginning.y}, checked_cells);
  }
  region.area += 1;
  region.perimeter += get_cell_perimeter(map, region_beginning, region_type, checked_cells);
  return region;
}

auto replace_used_cells(Map &map, const std::vector<Coordinates> &used_cells) -> void {
  for (const auto &cell : used_cells) {
    map[cell.x][cell.y] = '.';
  }
}

auto direction_changes(const Map &map, const Coordinates cell) -> size_t {
  const auto region_type = map[cell.x][cell.y];
  size_t changes = 0;

  const auto unknown_cell_on_the_right = !is_cell_on_the_right_in_region(map, cell, region_type);
  const auto unknown_cell_below = !is_cell_below_in_region(map, cell, region_type);
  const auto unknown_cell_on_the_left = !is_cell_on_the_left_in_region(map, cell, region_type);
  const auto unknown_cell_above = !is_cell_above_in_region(map, cell, region_type);
  const auto unknown_cell_on_upper_left_diagonal = !is_cell_on_upper_left_diagonal_in_region(map, cell, region_type);
  const auto unknown_cell_on_upper_right_diagonal = !is_cell_on_upper_right_diagonal_in_region(map, cell, region_type);
  const auto unknown_cell_on_lower_left_diagonal = !is_cell_on_lower_left_diagonal_in_region(map, cell, region_type);
  const auto unknown_cell_on_lower_right_diagonal = !is_cell_on_lower_right_diagonal_in_region(map, cell, region_type);

  if (unknown_cell_on_the_right && unknown_cell_below) {
    changes++;
  }
  if (unknown_cell_below && unknown_cell_on_the_left) {
    changes++;
  }
  if (unknown_cell_on_the_left && unknown_cell_above) {
    changes++;
  }
  if (unknown_cell_above && unknown_cell_on_the_right) {
    changes++;
  }
  if (!unknown_cell_on_the_left && !unknown_cell_above && unknown_cell_on_upper_left_diagonal) {
    changes++;
  }
  if (!unknown_cell_on_the_right && !unknown_cell_above && unknown_cell_on_upper_right_diagonal) {
    changes++;
  }
  if (!unknown_cell_on_the_left && !unknown_cell_below && unknown_cell_on_lower_left_diagonal) {
    changes++;
  }
  if (!unknown_cell_on_the_right && !unknown_cell_below && unknown_cell_on_lower_right_diagonal) {
    changes++;
  }

  return changes;
}

auto get_num_of_vertices(const Map &map, const std::vector<Coordinates> &cells) -> size_t {
  size_t vertices = 0;
  for (const auto &cell : cells) {
    vertices += direction_changes(map, cell);
  }
  return vertices;
}

auto get_fence_price(const Map &map, const bool part_2) -> size_t {
  auto modifiable_map = map;
  size_t fence_price = 0;
  for (auto region_beginning = find_region_beginning(modifiable_map); region_beginning.has_value();
       region_beginning = find_region_beginning(modifiable_map)) {
    std::vector<Coordinates> region_cells;
    const auto region = get_region(modifiable_map, region_beginning.value(), region_cells);
    replace_used_cells(modifiable_map, region_cells);
    if (!part_2) {
      fence_price += (region.perimeter * region.area);
    } else {
      fence_price += (get_num_of_vertices(map, region_cells) * region.area);
    }
  }
  return fence_price;
}

int main() {
  auto file = open_file(filename);
  auto file_as_string = file_to_string(file);
  file.close();
  std::cout << get_fence_price(file_as_string, false) << '\n'; // 1375476
  std::cout << get_fence_price(file_as_string, true) << '\n';  // 821372
  return 0;
}
