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
#include <cmath>

static constexpr std::string_view filename {"data.txt"};

using DataType = unsigned long long;
using Permutation = std::vector<std::string>;

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

DataType combine(const DataType &lhs, const DataType &rhs) {
  return std::stoull(std::to_string(lhs) + std::to_string(rhs));
}

auto get_all_operators_permutations(const size_t num_of_operations, const std::vector<std::string> &operators) -> std::vector<Permutation> {
  std::vector<Permutation> all_combinations;
  const size_t total_combinations = std::pow(operators.size(), num_of_operations);

  for (size_t i = 0; i < total_combinations; ++i) {
    Permutation current_combination;
    size_t index = i;
    for (size_t j = 0; j < num_of_operations; ++j) {
        current_combination.push_back(operators[index % operators.size()]);
        index /= operators.size();
    }
    all_combinations.push_back(current_combination);
  }
  return all_combinations;
}

auto get_result(const std::vector<DataType> &equation_constants, const std::vector<std::string> &operators) {
  DataType result = equation_constants[0];

  for (size_t i = 0; i < operators.size(); ++i) {
    if (operators[i] == "+") {
      result += equation_constants[i+1];
    } else if (operators[i] == "*") {
      result *= equation_constants[i+1];
    } else if (operators[i] == "||") {
      result = combine(result, equation_constants[i+1]);
    } else {
      throw std::runtime_error("Invalid operator");
    }
  }
  return result;
}

auto get_total_calibration_result(const std::vector<DataType> &equation_constants, const DataType correct_result, const std::vector<std::string> &operators) -> DataType {
  if (equation_constants.size() == 1) {
    return equation_constants[0] == correct_result ? correct_result : 0;
  }
  const auto num_of_operations = equation_constants.size() - 1;
  const auto operator_combinations = get_all_operators_permutations(num_of_operations, operators);
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

auto get_combined_calibration_result(const std::vector<std::vector<DataType>> &equations_with_results, const std::vector<std::string> &operators) {
  const auto [correct_results, constants] = split_equations(equations_with_results);
  DataType combined_calibration_result = 0;
  for (const auto &[equation, correct_result] : std::ranges::views::zip(constants, correct_results)) {
    combined_calibration_result += get_total_calibration_result(equation, correct_result, operators);    
  }
  return combined_calibration_result;
}

int main() {
  auto file = open_file(filename);
  auto lab_map = file_to_string(file);
  const auto parsed_equations = parse_equations(lab_map);
  std::cout << "Part 1: " << get_combined_calibration_result(parsed_equations, {"+", "*"}) << '\n';
  std::cout << "Part 2: " << get_combined_calibration_result(parsed_equations, {"+", "*", "||"}) << '\n';
  return 0;
}
