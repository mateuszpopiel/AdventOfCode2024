#include "bridge_repair.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

// TODO: REFACTOR REQUIRED, part 2 takes ~13s

using DataType = unsigned long long;
using Permutation = std::vector<std::string>;

auto parse_equations(const std::vector<std::string> &equations_str) {
  std::vector<std::vector<DataType>> equations;
  for (auto line : equations_str) {
    const auto colon_pos = line.find(':');
    line.erase(colon_pos, 1);
    std::istringstream line_stream(line);
    std::vector<DataType> equation;
    auto num = 0ull;
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

auto get_all_operators_permutations(const size_t num_of_operations,
                                    const std::vector<std::string> &operators) -> std::vector<Permutation> {
  std::vector<Permutation> all_combinations;
  const auto total_combinations = static_cast<size_t>(std::pow(operators.size(), num_of_operations));

  for (auto i = 0ull; i < total_combinations; ++i) {
    Permutation current_combination;
    auto index = i;
    for (auto j = 0ull; j < num_of_operations; ++j) {
      current_combination.push_back(operators[index % operators.size()]);
      index /= operators.size();
    }
    all_combinations.push_back(current_combination);
  }
  return all_combinations;
}

auto get_result(const std::vector<DataType> &equation_constants, const std::vector<std::string> &operators) {
  DataType result = equation_constants[0];

  for (auto i = 0ull; i < operators.size(); ++i) {
    if (operators[i] == "+") {
      result += equation_constants[i + 1];
    } else if (operators[i] == "*") {
      result *= equation_constants[i + 1];
    } else if (operators[i] == "||") {
      result = combine(result, equation_constants[i + 1]);
    } else {
      throw std::runtime_error("Invalid operator");
    }
  }
  return result;
}

auto get_total_calibration_result(const std::vector<DataType> &equation_constants, const DataType correct_result,
                                  const std::vector<std::string> &operators) -> DataType {
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

auto split_equations(const std::vector<std::vector<DataType>> &equations_with_results)
    -> std::vector<std::pair<std::vector<DataType>, DataType>> {
  std::vector<std::pair<std::vector<DataType>, DataType>> equations;
  equations.reserve(equations_with_results.size());
  for (const auto &equation : equations_with_results) {
    equations.emplace_back(std::vector<DataType>(equation.begin() + 1, equation.end()), equation.front());
  }
  return equations;
}

auto get_combined_calibration_result(const std::vector<std::vector<DataType>> &equations_with_results,
                                     const std::vector<std::string> &operators) -> ull {
  const auto equations = split_equations(equations_with_results);
  DataType combined_calibration_result = 0;
  for (const auto &[equation, correct_result] : equations) {
    combined_calibration_result += get_total_calibration_result(equation, correct_result, operators);
  }
  return combined_calibration_result;
}

ull solve(const bool part_2) {
  const auto lab_map = file_to_string("/home/mateusz/workspace/AdventOfCode2024/7/resources/data.txt");
  const auto parsed_equations = parse_equations(lab_map);
  const auto operators = (part_2 ? Permutation{"+", "*", "||"} : Permutation{"+", "*"});
  return get_combined_calibration_result(parsed_equations, operators);
}
