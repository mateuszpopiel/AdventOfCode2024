#include "race_condition.hpp"
#include "coordinates.hpp"
#include "file_helpers.hpp"
#include <array>
#include <cstdint>
#include <sstream>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

static constexpr std::array<Coordinates<int32_t>, 4> directions{Coordinates<int32_t>(0, -1), {0, 1}, {-1, 0}, {1, 0}};

std::tuple<Coordinates<int32_t>, std::unordered_set<Coordinates<int32_t>>> get_data(const std::string &filename) {
  std::string line;
  std::unordered_set<Coordinates<int32_t>> fields;
  Coordinates<int32_t> start;
  auto file = open_file(filename);
  for (auto x_id = 0; std::getline(file, line); ++x_id) {
    std::istringstream iss(line);
    char c = 0;
    for (auto y_id = 0; iss >> c; y_id++) {
      if (c == '#') {
        continue;
      }
      if (c == 'S') {
        start = {x_id, y_id};
      }
      fields.insert({x_id, y_id});
    }
  }
  file.close();
  return std::make_tuple(start, fields);
}

std::unordered_map<Coordinates<int32_t>, uint64_t> get_distances(const Coordinates<int32_t> start,
                                                                 const std::unordered_set<Coordinates<int32_t>> &grid) {
  std::stack<Coordinates<int32_t>> s;
  std::unordered_map<Coordinates<int32_t>, uint64_t> distances;

  s.push(start);
  distances[start] = 0;
  while (!s.empty()) {
    const auto current = s.top();
    s.pop();
    for (const auto &dir : directions) {
      const auto next = current + dir;
      if (grid.find(next) != grid.end() && distances.find(next) == distances.end()) {
        s.push(next);
        distances[next] = distances[current] + 1;
      }
    }
  }
  return distances;
}

std::tuple<uint64_t, uint64_t> get_number_of_cheats(const std::unordered_map<Coordinates<int32_t>, uint64_t> &distances,
                                                    const uint8_t distance_to_save) {
  std::vector<Coordinates<int32_t>> positions;
  positions.reserve(distances.size());
  for (const auto &[pos, _] : distances) {
    positions.push_back(pos);
  }
  const uint8_t allowed_skips_part_1 = 2;
  const uint8_t allowed_skips_part_2 = 20;
  uint64_t cheats_part_1 = 0;
  uint64_t cheats_part_2 = 0;

  for (auto i = 0u; i < distances.size(); ++i) {
    for (auto j = i + 1; j < distances.size(); ++j) {
      const auto first = positions.at(i);
      const auto second = positions.at(j);
      const auto distance = std::abs(first.x - second.x) + std::abs(first.y - second.y);
      const auto saved_distance =
          std::abs(static_cast<int64_t>(distances.at(second)) - static_cast<int64_t>(distances.at(first))) -
          static_cast<int64_t>(distance);
      if (distance <= allowed_skips_part_1 && saved_distance >= distance_to_save) {
        ++cheats_part_1;
      }
      if (distance <= allowed_skips_part_2 && saved_distance >= distance_to_save) {
        ++cheats_part_2;
      }
    }
  }
  return {cheats_part_1, cheats_part_2};
}

uint64_t solve(const bool part_2) {
  const auto [start, grid] = get_data("data.txt");
  const auto distances = get_distances(start, grid);
  const auto [cheats_p1, cheats_pq] = get_number_of_cheats(distances, 100u);
  return part_2 ? cheats_pq : cheats_p1;
}
