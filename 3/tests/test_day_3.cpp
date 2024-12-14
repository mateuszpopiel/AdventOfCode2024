#include "mull_it_over.hpp"
#include <gtest/gtest.h>

TEST(Day3, Part1) { EXPECT_EQ(191183308, solve(false)); }
TEST(Day3, Part2) { EXPECT_EQ(92082041, solve(true)); }
