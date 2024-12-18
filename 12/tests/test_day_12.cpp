#include "garden_groups.hpp"
#include <gtest/gtest.h>

TEST(Day12, Part1) { EXPECT_EQ(1375476, solve(false)); }
TEST(Day12, Part2) { EXPECT_EQ(821372, solve(true)); }
