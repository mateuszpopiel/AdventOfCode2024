#include "disk_fragmenter.hpp"
#include <gtest/gtest.h>

TEST(Day9, Part1) { EXPECT_EQ(6259790630969, solve(false)); }
TEST(Day9, Part2) { EXPECT_EQ(6289564433984, solve(true)); }
