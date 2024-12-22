#include "monkey_market.hpp"
#include <gtest/gtest.h>

TEST(Day22, Part1) { EXPECT_EQ(17262627539, solve(false)); }
TEST(Day22, Part2) { EXPECT_EQ(1986, solve(true)); }
