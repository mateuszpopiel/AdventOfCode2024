#include "bridge_repair.hpp"
#include <gtest/gtest.h>

TEST(Day7, Part1) { EXPECT_EQ(6231007345478, solve(false)); }
TEST(Day7, Part2) { EXPECT_EQ(333027885676693, solve(true)); }
