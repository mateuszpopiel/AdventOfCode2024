#include "template.hpp"
#include <gtest/gtest.h>

static constexpr auto example_result = (123u + 456u) * (98u - 76u);
TEST(Day15, Part1) { EXPECT_EQ(example_result, solve()); }
