#include "chronospatial_computer.hpp"
#include <iostream>

int main() {
  std::cout << solve(false) << '\n'; // 3,5,0,1,5,1,5,1,0
  std::cout << solve(true) << '\n';  // 107413700225434
  return 0;
}
