#include "restroom_doubt.hpp"
#include "coordinates.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <vector>

struct Robot {
  Coordinates<long long> position;
  Coordinates<long long> velocity;
};

static constexpr Coordinates<long long> world_size = {101, 103};

Robot get_robot_from_string(const std::string &robot_str) {
  Robot robot;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
  sscanf(robot_str.c_str(), "p=%lld,%lld v=%lld,%lld", &robot.position.x, &robot.position.y, &robot.velocity.x,
         &robot.velocity.y);
  return robot;
}

std::vector<Robot> get_robots(const std::vector<std::string> &robots_str) {
  std::vector<Robot> robots;
  robots.reserve(robots_str.size());
  for (const auto &robot_str : robots_str) {
    robots.push_back(get_robot_from_string(robot_str));
  }
  return robots;
}

std::vector<Robot> get_robots_from_file(const std::string_view &filename) {
  const auto robots_str = file_to_string(filename);
  return get_robots(robots_str);
}

void simulate_one_sec(std::vector<Robot> &robots) {
  for (auto &robot : robots) {
    robot.position.x = (robot.position.x + robot.velocity.x) % world_size.x;
    if (robot.position.x < 0) {
      robot.position.x += world_size.x;
    }
    robot.position.y = (robot.position.y + robot.velocity.y) % world_size.y;
    if (robot.position.y < 0) {
      robot.position.y += world_size.y;
    }
  }
}

Coordinates<long long> get_quadrant_begin(const Coordinates<long long> &world_size, const ull quadrant_number) {
  Coordinates<long long> quadrant_begin;
  switch (quadrant_number) {
  case 1:
    quadrant_begin = {0, 0};
    break;
  case 2:
    quadrant_begin = {0, world_size.y / 2 + 1};
    break;
  case 3:
    quadrant_begin = {world_size.x / 2 + 1, world_size.y / 2 + 1};
    break;
  case 4:
    quadrant_begin = {world_size.x / 2 + 1, 0};
    break;
  default:
    throw std::invalid_argument("Invalid quadrant number");
  }
  return quadrant_begin;
}

Coordinates<long long> get_quadrant_end(const Coordinates<long long> &world_size, const ull quadrant_number) {
  Coordinates<long long> quadrant_end;
  switch (quadrant_number) {
  case 1:
    quadrant_end = {world_size.x / 2, world_size.y / 2};
    break;
  case 2:
    quadrant_end = {world_size.x / 2, world_size.y};
    break;
  case 3:
    quadrant_end = {world_size.x, world_size.y};
    break;
  case 4:
    quadrant_end = {world_size.x, world_size.y / 2};
    break;
  default:
    throw std::invalid_argument("Invalid quadrant number");
  }
  return quadrant_end;
}

std::unordered_map<ull, size_t> get_number_of_robots_in_quadrants(const std::vector<Robot> &robots) {
  std::vector<std::pair<Coordinates<long long>, Coordinates<long long>>> quadrants;
  for (ull i = 1; i <= 4; ++i) {
    quadrants.emplace_back(get_quadrant_begin(world_size, i), get_quadrant_end(world_size, i));
  }
  std::unordered_map<ull, size_t> robots_in_quadrants;
  for (const auto &robot : robots) {
    for (ull i = 1; i < 5; ++i) {
      const auto &quadrant = quadrants[i - 1];
      if (robot.position.x >= quadrant.first.x && robot.position.x < quadrant.second.x &&
          robot.position.y >= quadrant.first.y && robot.position.y < quadrant.second.y) {
        robots_in_quadrants[i]++;
        break;
      }
    }
  }
  return robots_in_quadrants;
}

ull solve_part_1() {
  static constexpr auto time_to_simulate = 100u; // seconds
  auto robots = get_robots_from_file("data.txt");
  for (auto i = 0u; i < time_to_simulate; ++i) {
    simulate_one_sec(robots);
  }
  const auto robots_in_quadrants = get_number_of_robots_in_quadrants(robots);
  return std::accumulate(robots_in_quadrants.begin(), robots_in_quadrants.end(), 1u,
                         [](ull acc, const auto &robot_in_quadrant) { return acc * robot_in_quadrant.second; });
}

// NOLINTNEXTLINE(*-easily-swappable-parameters)
bool are_robots_aliged_horizontally(const std::vector<Robot> &robots, const ull aligned_robots, const ull times) {
  auto robots_aligned = 0u;
  for (auto i = 0u; i < world_size.x; ++i) {
    std::vector<Robot> robots_in_the_same_line;
    std::copy_if(robots.begin(), robots.end(), std::back_inserter(robots_in_the_same_line),
                 [i](const auto &robot) { return robot.position.x == i; });
    if (robots_in_the_same_line.size() < aligned_robots) {
      continue;
    }
    std::sort(robots_in_the_same_line.begin(), robots_in_the_same_line.end(),
              [](const auto &lhs, const auto &rhs) { return lhs.position.y < rhs.position.y; });
    auto robots_next_to_each_other = 0u;
    for (auto j = 0u; j < robots_in_the_same_line.size() - 1; ++j) {
      if (robots_in_the_same_line[j + 1].position.y - robots_in_the_same_line[j].position.y == 1) {
        robots_next_to_each_other++;
      }
      if (robots_next_to_each_other == aligned_robots) {
        robots_aligned++;
        if (robots_aligned == times) {
          return true;
        }
      }
    }
  }
  return false;
}

// NOLINTNEXTLINE(*-easily-swappable-parameters)
bool are_robots_aliged_vertically(const std::vector<Robot> &robots, const ull aligned_robots, const ull times) {
  auto robots_aligned = 0u;
  for (auto i = 0u; i < world_size.y; ++i) {
    std::vector<Robot> robots_in_the_same_line;
    std::copy_if(robots.begin(), robots.end(), std::back_inserter(robots_in_the_same_line),
                 [i](const auto &robot) { return robot.position.y == i; });
    if (robots_in_the_same_line.size() < aligned_robots) {
      continue;
    }
    std::sort(robots_in_the_same_line.begin(), robots_in_the_same_line.end(),
              [](const auto &lhs, const auto &rhs) { return lhs.position.x < rhs.position.x; });
    auto robots_next_to_each_other = 0u;
    for (auto j = 0u; j < robots_in_the_same_line.size() - 1; ++j) {
      if (robots_in_the_same_line[j + 1].position.x - robots_in_the_same_line[j].position.x == 1) {
        robots_next_to_each_other++;
      }
      if (robots_next_to_each_other == aligned_robots) {
        robots_aligned++;
        if (robots_aligned == times) {
          return true;
        }
      }
    }
  }
  return false;
}

bool are_robots_in_christmas_tree(const std::vector<Robot> &robots) {
  // Christmas tree is insiede the frame, check if robots are aligned horizontally and vertically twice
  return are_robots_aliged_horizontally(robots, 20, 3) && are_robots_aliged_vertically(robots, 20, 3);
}

ull solve_part_2() {
  static constexpr auto time_to_simulate = 10'000u; // seconds
  auto robots = get_robots_from_file("data.txt");
  for (auto i = 1u; i < time_to_simulate; ++i) {
    simulate_one_sec(robots);
    if (are_robots_in_christmas_tree(robots)) {
      return i;
    }
  }
  return time_to_simulate;
}

ull solve(const bool part_2) {
  if (!part_2) {
    return solve_part_1();
  }
  return solve_part_2();
}
