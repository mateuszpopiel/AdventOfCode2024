#include "lan_party.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>

TEST(Day23, Part1) { EXPECT_EQ(1119, get<uint64_t>(solve(false))); }
TEST(Day23, Part2) { EXPECT_EQ("av,fr,gj,hk,ii,je,jo,lq,ny,qd,uq,wq,xc", get<std::string>(solve(true))); }
