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

void print_permutations(const std::vector<std::vector<std::string>> &permutations) {
  for (const auto &permutation : permutations) {
    for (const auto &num : permutation) {
      std::cout << num << ' ';
    }
    std::cout << '\n';
  }
}

DataType combine(const DataType &lhs, const DataType &rhs) {
  return std::stoull(std::to_string(lhs) + std::to_string(rhs));
}

auto get_permutations(const Permutation initial_composition, const std::vector<std::string> &operators) {
  std::vector<Permutation> permutations;
  Permutation permutation = initial_composition;
  std::sort(permutation.begin(), permutation.end());
  do {
    permutations.push_back(permutation);
  } while (std::next_permutation(permutation.begin(), permutation.end()));
  return permutations;
}

auto get_all_operators_permutations(const size_t num_of_operations, const std::vector<std::string> &operators) -> std::vector<Permutation> {
  using Permutation = std::vector<std::string>;
  std::vector<Permutation> all_permutations;

  for (size_t op_id = 0; op_id < operators.size(); ++op_id) {
    Permutation initial_permutation(num_of_operations, operators[op_id]);
    const auto next_operator_id = (op_id + 1) % operators.size();
    for (size_t i = 0; i < num_of_operations; ++i) {
      initial_permutation[i] = operators[next_operator_id];
      const auto permutations = get_permutations(initial_permutation, operators);
      all_permutations.insert(all_permutations.end(), permutations.begin(), permutations.end());
    }
  }
  return all_permutations;
}

void print_equation(const std::vector<DataType> &equation_constants, const std::vector<std::string> &operators, DataType result) {
  for (size_t i = 0; i < equation_constants.size(); ++i) {
      std::cout << equation_constants[i];
      if (i < equation_constants.size() - 1) {
          std::cout << operators[i];
      }
  }
  std::cout << " = " << result << '\n';
}

auto get_result(const std::vector<DataType> &equation_constants, const std::vector<std::string> &operators) {
  DataType result;
  if (operators.front() == "+") {
    result = equation_constants[0] + equation_constants[1];
  } else if (operators.front() == "*") {
    result = equation_constants[0] * equation_constants[1];
  } else if (operators.front() == "||") {
    result = combine(equation_constants[0], equation_constants[1]);
  }

  for (size_t i = 2; i < equation_constants.size(); ++i) {
    if (operators[i - 1] == "+") {
      result += equation_constants[i];
    } else if (operators[i - 1] == "*") {
      result *= equation_constants[i];
    } else if (operators[i - 1] == "||") {
      result = combine(result, equation_constants[i]);
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
      print_equation(equation_constants, operators, correct_result);
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

auto count_correct_results_for_all_equations(const std::vector<std::vector<DataType>> &equations_with_results, const std::vector<std::string> &operators) {
  const auto [correct_results, constants] = split_equations(equations_with_results);
  DataType num_of_correct_results = 0;
  for (const auto &[equation, correct_result] : std::ranges::views::zip(constants, correct_results)) {
    num_of_correct_results += get_total_calibration_result(equation, correct_result, operators);    
  }
  return num_of_correct_results;
}

int main() {
  auto file = open_file(filename);
  auto lab_map = file_to_string(file);
  const auto parsed_equations = parse_equations(lab_map);
  std::cout << count_correct_results_for_all_equations(parsed_equations, {"+", "*"}) << '\n';
  return 0;
}
