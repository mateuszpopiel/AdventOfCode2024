#include "linean_layout.hpp"
#include <gtest/gtest.h>

TEST(Day19, Part1) { EXPECT_EQ(260, solve(false)); }
TEST(Day19, Part2) { EXPECT_EQ(639963796864990, solve(true)); }
