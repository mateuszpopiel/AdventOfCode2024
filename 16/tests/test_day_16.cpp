#include "reindeer_maze.hpp"
#include <gtest/gtest.h>

TEST(Day16, Part1) { EXPECT_EQ(88416, solve(false)); }
TEST(Day16, Part2) { EXPECT_EQ(442, solve(true)); }
