#include "claw_contraption.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 39290
  std::cout << solve(true) << '\n';  // 73458657399094
  return 0;
}
