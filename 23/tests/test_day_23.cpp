#include "lan_party.hpp"
#include <gtest/gtest.h>

TEST(Day23, Part1) { EXPECT_EQ(1119, solve(false)); }
TEST(Day23, Part2) { EXPECT_EQ("av,fr,gj,hk,ii,je,jo,lq,ny,qd,uq,wq,xc", solve(true)); }
