#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <sstream>
#include <array>
#include <string>
#include <utility>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <bitset>
#include <cmath>

static constexpr std::string_view filename {"data.txt"};

using DataType = unsigned long long;

auto open_file(const std::string_view &filename = filename) {
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file");
  }
  return file;
}

auto file_to_string(std::ifstream &file) {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }
  return lines;
}

auto parse_equations(const std::vector<std::string> &equations_str) {
    std::vector<std::vector<DataType>> equations;
    for (auto line : equations_str) {
      const auto colon_pos = line.find(':');
      line.erase(colon_pos, 1);
      std::istringstream line_stream(line);
      std::vector<DataType> equation;
      DataType num;
      while (line_stream >> num) {
        equation.push_back(num);
      }
      equations.push_back(equation);
    }
    return equations;
}

auto get_all_operators_permutations(size_t num_of_operators) {
  std::vector<std::string> all_permutations;
  for (size_t num_of_asterisks = 0; num_of_asterisks <= num_of_operators; ++num_of_asterisks) {
    std::string permutation(num_of_operators - num_of_asterisks, '+');
    permutation += std::string(num_of_asterisks, '*');
    std::sort(permutation.begin(), permutation.end());
    do {
      all_permutations.push_back(permutation);
    } while (std::next_permutation(permutation.begin(), permutation.end()));
  }
  if (all_permutations.size() < pow(2, num_of_operators)) {
    throw std::runtime_error("Generated less permutations than expected");
  }
  return all_permutations;
}

void print_equation(const std::vector<DataType> &equation_constants, const std::string &operators, DataType result) {
  for (size_t i = 0; i < equation_constants.size(); ++i) {
      std::cout << equation_constants[i];
      if (i < equation_constants.size() - 1) {
          std::cout << operators[i];
      }
  }
  std::cout << " = " << result << '\n';
}

auto get_result(const std::vector<DataType> &equation_constants, const std::string &operators) {
  auto result = operators[0] == '+'
    ? equation_constants[0] + equation_constants[1]
    : equation_constants[0] * equation_constants[1];
  for (size_t i = 2; i < equation_constants.size(); ++i) {
    result = operators[i - 1] == '+'
      ? result + equation_constants[i]
      : result * equation_constants[i];
  }
  //print_equation(equation_constants, operators, result);
  return result;
}

auto get_total_calibration_result(const std::vector<DataType> &equation_constants, const DataType correct_result) -> DataType {
  if (equation_constants.size() == 1) {
    return equation_constants[0] == correct_result ? correct_result : 0;
  }
  const auto num_of_operators = equation_constants.size() - 1;
  const auto operator_combinations = get_all_operators_permutations(num_of_operators);
  for (const auto &operators : operator_combinations) {
    if (get_result(equation_constants, operators) == correct_result) {
      return correct_result;
    }
  }
  return 0;
}

auto split_equations(const std::vector<std::vector<DataType>> &equations_with_results) {
  std::vector<DataType> results;
  std::vector<std::vector<DataType>> constants;

  for (const auto &equation : equations_with_results) {
      results.push_back(equation.front());
      constants.push_back(std::vector<DataType>(equation.begin() + 1, equation.end()));
  }

  return std::make_pair(results, constants);
}

auto count_correct_results_for_all_equations(const std::vector<std::vector<DataType>> &equations_with_results) {
  const auto [correct_results, constants] = split_equations(equations_with_results);
  DataType num_of_correct_results = 0;
  for (const auto &[equation, correct_result] : std::ranges::views::zip(constants, correct_results)) {
    num_of_correct_results += get_total_calibration_result(equation, correct_result);    
  }
  return num_of_correct_results;
}

int main() {
  auto file = open_file(filename);
  auto lab_map = file_to_string(file);
  const auto parsed_equations = parse_equations(lab_map);
  std::cout << count_correct_results_for_all_equations(parsed_equations) << '\n';
  return 0;
}
