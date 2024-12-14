#include "template.hpp"
#include <gtest/gtest.h>

static constexpr auto example_result_part_1 = (123u + 45u) * (98u - 76u);
static constexpr auto example_result_part_2 = (123u - 45u) + (98u * 76u);
TEST(Day0, Part1) { EXPECT_EQ(example_result_part_1, solve(false)); }
TEST(Day0, Part2) { EXPECT_EQ(example_result_part_2, solve(true)); }
