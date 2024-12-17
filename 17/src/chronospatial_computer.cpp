#include "chronospatial_computer.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::pair<std::array<uint64_t, 3>, std::vector<uint8_t>> get_data(const std::string &filename) {
  auto file = open_file(filename);
  std::array<uint64_t, 3> registers{0, 0, 0};
  std::vector<uint8_t> program;

  std::string line;
  for (auto &reg : registers) {
    std::getline(file, line);
    std::istringstream iss(line.substr(line.find(':') + 1));
    iss >> reg;
  }
  while (std::getline(file, line)) {
    if (line.find("Program:") == std::string::npos) {
      continue;
    }
    std::istringstream iss(line.substr(line.find(':') + 1));
    uint8_t value = 0;
    while (iss >> value) {
      if (value == ',') {
        continue;
      }
      program.push_back(value - '0');
    }
  }

  file.close();
  return std::make_pair(registers, program);
}

void print_code(const std::array<uint64_t, 3> &registers, const std::vector<uint8_t> &program) {
  for (const auto reg : registers) {
    std::cout << reg << " ";
  }
  std::putc('\n', stdout);
  for (const auto instr : program) {
    std::cout << instr << " ";
  }
  std::putc('\n', stdout);
}

uint64_t get_combo_operand(const uint8_t instr, const std::array<uint64_t, 3> &registers) {
  if (instr < 4) {
    return static_cast<uint64_t>(instr);
  }
  if (instr > 6) {
    throw std::runtime_error("Program invalid, trying to get combo operand for invalid instruction");
  }
  // NOLINTNEXTLINE(*constant-array-index)
  return registers[instr - 4u];
}

void adv(std::array<uint64_t, 3> &registers, const uint8_t operand) {
  registers[0] = registers[0] / (1ull << get_combo_operand(operand, registers));
}

void bxl(std::array<uint64_t, 3> &registers, const uint8_t operand) {
  registers[1] = registers[1] ^ static_cast<uint64_t>(operand);
}

void bst(std::array<uint64_t, 3> &registers, const uint8_t operand) {
  registers[1] = get_combo_operand(operand, registers) % 8;
}

bool jnz(const std::array<uint64_t, 3> &registers, const uint8_t operand,
         std::vector<uint8_t>::const_iterator &instr_ptr, const std::vector<uint8_t>::const_iterator &begin) {
  if (registers[0] == 0) {
    return false;
  }
  instr_ptr = std::next(begin, operand);
  return true;
}

void bxc(std::array<uint64_t, 3> &registers) { registers[1] = (registers[1] ^ registers[2]); }

uint8_t out(const std::array<uint64_t, 3> &registers, const uint8_t operand) {
  return static_cast<uint8_t>(get_combo_operand(operand, registers) % 8);
}

void bdv(std::array<uint64_t, 3> &registers, const uint8_t operand) {
  registers[1] = registers[0] / (1ull << get_combo_operand(operand, registers));
}

void cdv(std::array<uint64_t, 3> &registers, const uint8_t operand) {
  registers[2] = registers[0] / (1ull << get_combo_operand(operand, registers));
}

std::vector<uint8_t> execute_program(const uint64_t a, const std::vector<uint8_t> &program) {
  const uint64_t b = 0;
  const uint64_t c = 0;
  std::array<uint64_t, 3> registers{a, b, c};
  bool jumped = false;
  std::vector<uint8_t> output;

  for (auto instr_ptr = program.begin(); instr_ptr < program.end();) {
    if (std::distance(instr_ptr, program.end()) < 2) {
      throw std::runtime_error("Program invalid, incomplete instruction");
    }

    const auto opcode = *instr_ptr;
    const auto operand = *(std::next(instr_ptr));
    switch (opcode) {
    case 0:
      adv(registers, operand);
      break;
    case 1:
      bxl(registers, operand);
      break;
    case 2:
      bst(registers, operand);
      break;
    case 3:
      jumped = jnz(registers, operand, instr_ptr, program.begin());
      break;
    case 4:
      bxc(registers);
      break;
    case 5:
      output.push_back(out(registers, operand));
      break;
    case 6:
      bdv(registers, operand);
      break;
    case 7:
      cdv(registers, operand);
      break;
    default:
      throw std::runtime_error("Program invalid, invalid opcode");
    }
    if (jumped) {
      jumped = false;
      continue;
    }
    instr_ptr = std::next(instr_ptr, 2);
  }
  return output;
}

uint64_t restore_reg_a(const std::vector<uint8_t> &program, uint64_t a = 0, uint64_t depth = 0) {
  if (depth == program.size()) {
    return a;
  }
  for (uint8_t i = 0; i < 8; ++i) {
    const auto output = execute_program(a * 8 + i, program);
    if (!output.empty() && output.front() == program[program.size() - 1 - depth]) {
      const auto result = restore_reg_a(program, a * 8 + i, depth + 1);
      if (result != 0) {
        return result;
      }
    }
  }
  return 0;
}

std::string output_to_string(const std::vector<uint8_t> &output) {
  std::string output_str;
  for (size_t i = 0; i < output.size(); ++i) {
    output_str += std::to_string(output[i]);
    if (i != output.size() - 1) {
      output_str += ",";
    }
  }
  return output_str;
}

std::string solve(const bool part_2) {
  const auto [registers, program] = get_data("data.txt");
  if (!part_2) {
    const auto output = execute_program(registers[0], program);
    return output_to_string(output);
  }
  return std::to_string(restore_reg_a(program));
}
