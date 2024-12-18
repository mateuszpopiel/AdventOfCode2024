#include "resonant_collinearity.hpp"
#include <gtest/gtest.h>

TEST(Day8, Part1) { EXPECT_EQ(295, solve(false)); }
TEST(Day8, Part2) { EXPECT_EQ(1034, solve(true)); }
