#include "ceres_search.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 2545
  std::cout << solve(true) << '\n';  // 1886
  return 0;
}
