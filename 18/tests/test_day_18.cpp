#include "ram_run.hpp"
#include <gtest/gtest.h>

TEST(Day18, Part1) { EXPECT_EQ(278, std::get<ull>(solve(false))); }
TEST(Day18, Part2) { EXPECT_EQ("43,12", std::get<std::string>(solve(true))); }
