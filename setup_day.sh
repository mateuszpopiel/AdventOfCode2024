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
echo "123 456" > $day_nr/resources/data.txt
echo "98 76" >> $day_nr/resources/data.txt

echo "Writing $day_theme.hpp file"
echo "#pragma once" > $day_nr/inc/$day_theme.hpp
echo "using ull = unsigned long long;" >> $day_nr/inc/$day_theme.hpp
echo "ull solve();" >> $day_nr/inc/$day_theme.hpp

echo "Writing $day_theme.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/src/$day_theme.cpp
echo "#include \"file_helpers.hpp\"" >> $day_nr/src/$day_theme.cpp
echo "#include <sstream>" >> $day_nr/src/$day_theme.cpp
echo "#include <string>" >> $day_nr/src/$day_theme.cpp
echo "#include <vector>" >> $day_nr/src/$day_theme.cpp
echo "" >> $day_nr/src/$day_theme.cpp
echo "std::vector<std::vector<ull>> get_data(const std::string &filename) {" >> $day_nr/src/$day_theme.cpp
echo "  std::vector<std::vector<ull>> result;" >> $day_nr/src/$day_theme.cpp
echo "  const auto content = get_input_from_multiline_file(filename);" >> $day_nr/src/$day_theme.cpp
echo "  for (const auto &row : content) {" >> $day_nr/src/$day_theme.cpp
echo "    auto iss = std::istringstream(row);" >> $day_nr/src/$day_theme.cpp
echo "    std::vector<ull> row_data;" >> $day_nr/src/$day_theme.cpp
echo "    ull value = 0;" >> $day_nr/src/$day_theme.cpp
echo "    while (iss >> value) {" >> $day_nr/src/$day_theme.cpp
echo "      row_data.push_back(value);" >> $day_nr/src/$day_theme.cpp
echo "    }" >> $day_nr/src/$day_theme.cpp
echo "    result.push_back(row_data);" >> $day_nr/src/$day_theme.cpp
echo "  }" >> $day_nr/src/$day_theme.cpp
echo "  return result;" >> $day_nr/src/$day_theme.cpp
echo "}" >> $day_nr/src/$day_theme.cpp
echo "" >> $day_nr/src/$day_theme.cpp
echo "ull solve() {" >> $day_nr/src/$day_theme.cpp
echo "  const auto data = get_data(\"data.txt\");" >> $day_nr/src/$day_theme.cpp
echo "  return (data[0][0] + data[0][1]) * (data[1][0] - data[1][1]);" >> $day_nr/src/$day_theme.cpp
echo "}" >> $day_nr/src/$day_theme.cpp

echo "Writing main.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/src/main.cpp
echo "#include <iostream>" >> $day_nr/src/main.cpp
echo "" >> $day_nr/src/main.cpp
echo "int main() {" >> $day_nr/src/main.cpp
echo "  std::cout << \"Result: \" << solve() << std::endl;" >> $day_nr/src/main.cpp
echo "  return 0;" >> $day_nr/src/main.cpp
echo "}" >> $day_nr/src/main.cpp

echo "Writing test_day_$day_nr.cpp file"
echo "#include \"$day_theme.hpp\"" > $day_nr/tests/test_day_$day_nr.cpp
echo "#include <gtest/gtest.h>" >> $day_nr/tests/test_day_$day_nr.cpp
echo "" >> $day_nr/tests/test_day_$day_nr.cpp
echo "static constexpr auto example_result = (123u + 456u) * (98u - 76u);" >> $day_nr/tests/test_day_$day_nr.cpp
echo "TEST(Day$day_nr, Part1) { EXPECT_EQ(example_result, solve()); }" >> $day_nr/tests/test_day_$day_nr.cpp

echo "Done! Now you can start solving day $day_nr with theme $day_theme"
echo "Don't forget to add the new day to the main CMakeLists.txt file"
