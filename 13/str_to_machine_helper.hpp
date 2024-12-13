#include "../lib/inc/file_helpers.hpp"
#include <iostream>
#include <sstream>
#include <optional>

struct Coordinates {
  size_t x;
  size_t y;

  constexpr Coordinates operator+(const Coordinates &other) const {
    return {x + other.x, y + other.y};
  }

  constexpr Coordinates operator*(const size_t scalar) const {
    return {x * scalar, y * scalar};
  }

  constexpr bool operator==(const Coordinates &other) const {
    return x == other.x && y == other.y;
  }
};

struct Machine {
  Coordinates button_a;
  Coordinates button_b;
  Coordinates prize;
};

std::optional<Coordinates> get_button(std::istringstream &iss);
std::optional<Coordinates> get_prize(std::istringstream &iss);
std::vector<Machine> get_machines(const std::vector<std::string> &input);
void print_machine(const Machine &machine);
