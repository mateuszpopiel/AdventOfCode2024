#include "lan_party.hpp"
#include <iostream>

int main() {
  std::cout << get<uint64_t>(solve(false)) << '\n';   // 1119
  std::cout << get<std::string>(solve(true)) << '\n'; // "av,fr,gj,hk,ii,je,jo,lq,ny,qd,uq,wq,xc"
  return 0;
}
