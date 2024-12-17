#include "print_queue.hpp"
#include <gtest/gtest.h>

TEST(Day5, Part1) { EXPECT_EQ(5964, solve(false)); }
TEST(Day5, Part2) { EXPECT_EQ(4719, solve(true)); }
