#include "claw_contraption.hpp"
#include <gtest/gtest.h>

TEST(Day13, Part1) { EXPECT_EQ(39290, solve(false)); }
TEST(Day13, Part2) { EXPECT_EQ(73458657399094, solve(true)); }
