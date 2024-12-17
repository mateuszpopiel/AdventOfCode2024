#include "ceres_search.hpp"
#include <gtest/gtest.h>

TEST(Day4, Part1) { EXPECT_EQ(2545, solve(false)); }
TEST(Day4, Part2) { EXPECT_EQ(1886, solve(true)); }
