#include "chronospatial_computer.hpp"
#include <gtest/gtest.h>

TEST(Day17, Part1) { EXPECT_EQ("3,5,0,1,5,1,5,1,0", solve(false)); }
TEST(Day17, Part2) { EXPECT_EQ("107413700225434", solve(true)); }
