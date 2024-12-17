#include "restroom_doubt.hpp"
#include <gtest/gtest.h>

TEST(Day14, Part1) { EXPECT_EQ(217132650, solve(false)); }
TEST(Day14, Part2) { EXPECT_EQ(6516, solve(true)); }
