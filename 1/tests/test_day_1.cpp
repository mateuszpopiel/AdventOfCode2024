#include "historian_hysteria.hpp"
#include <gtest/gtest.h>

TEST(Day1, Part1) { ASSERT_EQ(2430334, solve(false)); }
TEST(Day1, Part2) { ASSERT_EQ(28786472, solve(true)); }
