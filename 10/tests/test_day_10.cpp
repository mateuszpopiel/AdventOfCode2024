#include "hoof_it.hpp"
#include <gtest/gtest.h>

TEST(Day10, Part1) { EXPECT_EQ(709, solve(false)); }
TEST(Day10, Part2) { EXPECT_EQ(1326, solve(true)); }
