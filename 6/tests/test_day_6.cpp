#include "guard_gallivand.hpp"
#include <gtest/gtest.h>

TEST(Day6, Part1) { EXPECT_EQ(5212, solve(false)); }
TEST(Day6, Part2) { EXPECT_EQ(1767, solve(true)); }
