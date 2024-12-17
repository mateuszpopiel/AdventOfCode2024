#include "coordinates.hpp"
#include "file_helpers.hpp"
#include <iostream>
#include <optional>
#include <sstream>

struct Machine {
  Coordinates<long long> button_a;
  Coordinates<long long> button_b;
  Coordinates<long long> prize;
};

std::optional<Coordinates<long long>> get_button(std::istringstream &iss);
std::optional<Coordinates<long long>> get_prize(std::istringstream &iss);
std::vector<Machine> get_machines(const std::vector<std::string> &input, const bool part_2);
void print_machine(const Machine &machine);
