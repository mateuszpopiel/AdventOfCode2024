#include "race_condition.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 1511
  std::cout << solve(true) << '\n';  // 1'020'507
  return 0;
}
