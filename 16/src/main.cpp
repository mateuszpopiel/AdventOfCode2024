#include "reindeer_maze.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 88416
  std::cout << solve(true) << '\n';  // 442
  return 0;
}
