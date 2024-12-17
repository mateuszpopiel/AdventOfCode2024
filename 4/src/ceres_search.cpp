#include "ceres_search.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <string>
#include <vector>

std::vector<std::vector<ull>> get_data(const std::string &filename) {
  auto file = open_file(filename);
  auto result = parse_file<ull>(file); // automatic move
  file.close();
  return result;
}

auto multi_line_string_to_one_line(const std::vector<std::string> &lines) {
  std::string one_line;
  for (const auto &line : lines) {
    one_line += line + ' ';
  }
  return one_line;
}

auto multi_line_string_to_one_line_vertically(const std::vector<std::string> &lines) {
  std::string one_line;
  for (auto id = 0u; id < lines[0].size(); ++id) {
    for (const auto &line : lines) {
      one_line += line[id];
    }
    one_line += ' ';
  }
  return one_line;
}

auto multi_line_string_to_one_line_diagonally(const std::vector<std::string> &lines) {
  std::string one_line;
  for (auto diag_id = 0u; diag_id < 2 * lines.size() - 1; ++diag_id) {
    for (auto row_id = 0u; row_id < lines.size(); ++row_id) {
      auto col_id = diag_id - row_id;
      if (col_id < lines.size()) {
        one_line += lines[row_id][col_id];
      }
    }
    one_line += ' ';
  }
  return one_line;
}

auto multi_line_string_to_one_line_diagonally_other_way(const std::vector<std::string> &lines) {
  std::string one_line;
  for (auto diag_id = 0u; diag_id < 2 * lines.size() - 1; ++diag_id) {
    for (auto row_id = 0u; row_id < lines.size(); ++row_id) {
      auto col_id = lines.size() - 1 - (diag_id - row_id);
      if (col_id < lines.size()) {
        one_line += lines[row_id][col_id];
      }
    }
    one_line += ' ';
  }
  return one_line;
}

auto reverse_string(const std::string &str) {
  std::string reversed_str = str;
  std::reverse(reversed_str.begin(), reversed_str.end());
  return reversed_str;
}

std::array<std::string, 8> get_all_strings(const std::vector<std::string> &file_as_str) {
  auto forward_str = multi_line_string_to_one_line(file_as_str);
  auto backward_str = reverse_string(forward_str);
  auto vertical_str = multi_line_string_to_one_line_vertically(file_as_str);
  auto reversed_vertical_str = reverse_string(vertical_str);
  auto diagonal_str = multi_line_string_to_one_line_diagonally(file_as_str);
  auto reversed_diagonal_str = reverse_string(diagonal_str);
  auto diagonal_str_other_way = multi_line_string_to_one_line_diagonally_other_way(file_as_str);
  auto reversed_diagonal_str_other_way = reverse_string(diagonal_str_other_way);
  return {forward_str,  backward_str,          vertical_str,           reversed_vertical_str,
          diagonal_str, reversed_diagonal_str, diagonal_str_other_way, reversed_diagonal_str_other_way};
}

auto count_xmas(const std::string &str) {
  ull count = 0;
  for (auto pos = str.find("XMAS"); pos != std::string::npos; ++count) {
    pos = str.find("XMAS", pos + 4);
  }
  return count;
}

auto count_xmas_in_multiple_strings(const std::array<std::string, 8> &strings) {
  ull count = 0;
  for (const auto &str : strings) {
    count += count_xmas(str);
  }
  return count;
}

auto is_x_mas(const std::array<std::array<char, 3>, 3> &lines) {
  return (lines[0][0] == 'M' && lines[0][2] == 'M' && lines[1][1] == 'A' && lines[2][0] == 'S' && lines[2][2] == 'S') ||
         (lines[0][0] == 'S' && lines[0][2] == 'S' && lines[1][1] == 'A' && lines[2][0] == 'M' && lines[2][2] == 'M') ||
         (lines[0][0] == 'M' && lines[0][2] == 'S' && lines[1][1] == 'A' && lines[2][0] == 'M' && lines[2][2] == 'S') ||
         (lines[0][0] == 'S' && lines[0][2] == 'M' && lines[1][1] == 'A' && lines[2][0] == 'S' && lines[2][2] == 'M');
}

auto count_x_mas(const std::vector<std::string> &lines) {
  ull count = 0;
  for (auto row_id = 0u; row_id < lines.size() - 2; ++row_id) {
    for (auto col_id = 0u; col_id < lines[0].size() - 2; ++col_id) {
      const std::array<std::array<char, 3>, 3> subgrid = {
          {{lines[row_id][col_id], lines[row_id][col_id + 1], lines[row_id][col_id + 2]},
           {lines[row_id + 1][col_id], lines[row_id + 1][col_id + 1], lines[row_id + 1][col_id + 2]},
           {lines[row_id + 2][col_id], lines[row_id + 2][col_id + 1], lines[row_id + 2][col_id + 2]}}};
      if (is_x_mas(subgrid)) {
        ++count;
      }
    }
  }
  return count;
}

ull solve(const bool part_2) {
  const auto file_as_str = get_input_from_multiline_file("data.txt");
  const auto strings = get_all_strings(file_as_str);
  if (!part_2) {
    return count_xmas_in_multiple_strings(strings);
  }
  return count_x_mas(file_as_str);
}
