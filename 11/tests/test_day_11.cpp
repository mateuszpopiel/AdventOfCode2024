#include "plutionian_pebbles.hpp"
#include <gtest/gtest.h>

TEST(Day11, Part1) { EXPECT_EQ(199753, solve(false)); }
TEST(Day11, Part2) { EXPECT_EQ(239413123020116, solve(true)); }
