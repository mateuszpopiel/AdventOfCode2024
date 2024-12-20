#include "race_condition.hpp"
#include <gtest/gtest.h>

TEST(Day20, Part1) { EXPECT_EQ(1511, solve(false)); }
TEST(Day20, Part2) { EXPECT_EQ(1'020'507, solve(true)); }
