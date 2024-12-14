#
# Script for creating all required directories and files for a new day.
# It prepares CMakeLists.txt and template *.cpp files based on its arguments
# Usage:
#   ./setup_day.sh <day_nr> <day_theme>
# Example:
#   ./setup_day.sh 1 "historian_hysteria"
#
#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./setup_day.sh <day_nr> <day_theme>"
    exit 1
fi

day_nr=$1
day_theme=$2

if [ ! -d "$day_nr" ]; then
    mkdir $day_nr
fi

echo "Creating directories and files for day $day_nr with theme $day_theme"
mkdir -p $day_nr/inc $day_nr/resources $day_nr/src $day_nr/tests
touch $day_nr/CMakeLists.txt $day_nr/inc/$day_theme.hpp $day_nr/src/$day_theme.cpp \
      $day_nr/src/main.cpp $day_nr/tests/test_day_$day_nr.cpp $day_nr/resources/data.txt

echo "Writing CMakeLists.txt file"
echo "day_setup(day_$day_nr $day_theme)" > $day_nr/CMakeLists.txt

echo "Writing template content to data.txt file"
echo "123 45" > $day_nr/resources/data.txt
echo "98 76" >> $day_nr/resources/data.txt

echo "Writing $day_theme.hpp file"
echo "#pragma once" > $day_nr/inc/$day_theme.hpp
echo "using ull = unsigned long long;" >> $day_nr/inc/$day_theme.hpp
echo "ull solve(const bool part_2);" >> $day_nr/inc/$day_theme.hpp

echo "Writing $day_theme.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/src/$day_theme.cpp
echo "#include \"file_helpers.hpp\"" >> $day_nr/src/$day_theme.cpp
echo "#include <sstream>" >> $day_nr/src/$day_theme.cpp
echo "#include <string>" >> $day_nr/src/$day_theme.cpp
echo "#include <vector>" >> $day_nr/src/$day_theme.cpp
echo "" >> $day_nr/src/$day_theme.cpp
echo "std::vector<std::vector<ull>> get_data(const std::string &filename) {" >> $day_nr/src/$day_theme.cpp
echo "  auto file = open_file(filename);" >> $day_nr/src/$day_theme.cpp
echo "  // NOLINTNEXTLINE(*no-automatic-move)" >> $day_nr/src/$day_theme.cpp
echo "  const auto result = parse_file<ull>(file);" >> $day_nr/src/$day_theme.cpp
echo "  file.close();" >> $day_nr/src/$day_theme.cpp
echo "  return result;" >> $day_nr/src/$day_theme.cpp
echo "}" >> $day_nr/src/$day_theme.cpp
echo "" >> $day_nr/src/$day_theme.cpp
echo "ull solve(const bool part_2) {" >> $day_nr/src/$day_theme.cpp
echo "  const auto data = get_data(\"data.txt\");" >> $day_nr/src/$day_theme.cpp
echo "  if (!part_2) {" >> $day_nr/src/$day_theme.cpp
echo "    return (data[0][0] + data[0][1]) * (data[1][0] - data[1][1]);" >> $day_nr/src/$day_theme.cpp
echo "  }" >> $day_nr/src/$day_theme.cpp
echo "  return (data[0][0] - data[0][1]) + (data[1][0] * data[1][1]);" >> $day_nr/src/$day_theme.cpp
echo "}" >> $day_nr/src/$day_theme.cpp

echo "Writing main.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/src/main.cpp
echo "#include <iostream>" >> $day_nr/src/main.cpp
echo "" >> $day_nr/src/main.cpp
echo "int main() {" >> $day_nr/src/main.cpp
echo "  std::cout << \"Result: \" << solve(false) << '\n';" >> $day_nr/src/main.cpp
echo "  std::cout << \"Result: \" << solve(true) << '\n';" >> $day_nr/src/main.cpp
echo "  return 0;" >> $day_nr/src/main.cpp
echo "}" >> $day_nr/src/main.cpp

echo "Writing test_day_$day_nr.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/tests/test_day_$day_nr.cpp
echo "#include <gtest/gtest.h>" >> $day_nr/tests/test_day_$day_nr.cpp
echo "" >> $day_nr/tests/test_day_$day_nr.cpp
echo "static constexpr auto example_result_part_1 = (123u + 45u) * (98u - 76u);" >> $day_nr/tests/test_day_$day_nr.cpp
echo "static constexpr auto example_result_part_2 = (123u - 45u) + (98u * 76u);" >> $day_nr/tests/test_day_$day_nr.cpp
echo "TEST(Day$day_nr, Part1) { EXPECT_EQ(example_result_part_1, solve(false)); }" >> $day_nr/tests/test_day_$day_nr.cpp
echo "TEST(Day$day_nr, Part2) { EXPECT_EQ(example_result_part_2, solve(true)); }" >> $day_nr/tests/test_day_$day_nr.cpp

echo "Done! Now you can start solving day $day_nr with theme $day_theme"
echo "Don't forget to add the new day to the main CMakeLists.txt file"
