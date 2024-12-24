#include "crossed_wires.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

std::pair<std::vector<std::string>, std::vector<std::string>> split_input(const std::string &filename) {
  const auto input = file_to_string(filename);
  const auto split_sign_iter = std::find(input.begin(), input.end(), "");
  return std::make_pair(std::vector<std::string>{input.begin(), split_sign_iter},
                        std::vector<std::string>{std::next(split_sign_iter), input.end()});
}

std::map<std::string, bool> get_inputs(const std::vector<std::string> &inputs) {
  std::map<std::string, bool> inputs_map;
  for (const auto &line : inputs) {
    std::string wire_id;
    bool wire_val = false;
    std::istringstream iss(line);
    iss >> wire_id >> wire_val;
    wire_id.erase(std::prev(wire_id.end())); // the ':'
    inputs_map[wire_id] = wire_val;
  }
  return inputs_map;
}

bool compute(const std::string &in1, const std::string &in2, const std::string &op,
             const std::map<std::string, bool> &known) {
  if (op == "AND") {
    return known.at(in1) && known.at(in2);
  }
  if (op == "OR") {
    return known.at(in1) || known.at(in2);
  }
  if (op == "XOR") {
    return known.at(in1) ^ known.at(in2);
  }
  throw "Invalid operator";
}

std::tuple<std::string, std::string, std::string, std::string> parse_line(const std::string &line) {
  std::istringstream iss(line);
  std::string wire_1;
  std::string wire_2;
  std::string out_wire;
  std::string op;
  std::string trash;
  iss >> wire_1 >> op >> wire_2 >> trash >> out_wire;
  return std::make_tuple(wire_1, wire_2, op, out_wire);
}

std::map<std::string, bool> filter_known_wires(const std::map<std::string, bool> &known) {
  std::map<std::string, bool> outputs;
  for (const auto &[name, val] : known) {
    if (name[0] == 'z') {
      outputs[name] = val;
    }
  }
  return outputs;
}

std::map<std::string, bool> get_outputs(std::map<std::string, bool> known_map, std::vector<std::string> outputs_str) {
  std::set<std::string> known;

  for (const auto &[name, _] : known_map) {
    known.insert(name);
  }

  // NOLINTNEXTLINE(*-avoid-do-while)
  do {
    std::vector<std::string> unused_lines;
    for (const auto &line : outputs_str) {
      const auto [wire_1, wire_2, op, out_wire] = parse_line(line);
      if (!known.contains(wire_1) || !known.contains(wire_2)) {
        unused_lines.push_back(line);
        continue;
      }
      known_map[out_wire] = compute(wire_1, wire_2, op, known_map);
      known.insert(out_wire);
    }
    outputs_str = unused_lines;
  } while (!outputs_str.empty());
  return filter_known_wires(known_map);
}

uint64_t combine_bits(const std::map<std::string, bool> &outputs) {
  uint64_t out = 0;
  std::vector<bool> bits;
  bits.reserve(outputs.size());
  for (const auto &[_, bit] : outputs) {
    bits.push_back(bit);
  }
  std::reverse(bits.begin(), bits.end());
  for (const bool bit : bits) {
    out = (out << 1) | static_cast<uint64_t>(bit);
  }
  return out;
}

uint64_t solve(const bool /*part_2*/) {
  const auto [inputs_str, outputs_str] = split_input("data.txt");
  const auto inputs = get_inputs(inputs_str);
  const auto outputs = get_outputs(inputs, outputs_str);
  return combine_bits(outputs);
}
