#include "ram_run.hpp"
#include <iostream>

int main() {
  const auto part_1_solution = solve(false);
  std::cout << std::get<ull>(part_1_solution) << '\n';
  std::cout << std::get<std::string>(solve(true)) << '\n';
  return 0;
}
