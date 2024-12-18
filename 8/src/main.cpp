#include "resonant_collinearity.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 295
  std::cout << solve(true) << '\n';  // 1034
  return 0;
}
