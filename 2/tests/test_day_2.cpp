#include "red-nosed_report.hpp"
#include <gtest/gtest.h>

TEST(Day2, Part1) { EXPECT_EQ(269, solve(false)); }
TEST(Day2, Part2) { EXPECT_EQ(337, solve(true)); }
