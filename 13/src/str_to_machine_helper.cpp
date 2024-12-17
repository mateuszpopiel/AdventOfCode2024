#include "str_to_machine_helper.hpp"

inline constexpr long long unit_conversion_shift = 10'000'000'000'000;

auto get_button(std::istringstream &iss) -> std::optional<Coordinates<long long>> {
  std::string temp;
  std::string x;
  std::string y;
  Coordinates<long long> button;

  //     Button  A:     X+94, Y+34
  iss >> temp >> temp >> x >> y;
  if (temp != "A:" && temp != "B:") {
    return std::nullopt;
  }
  x = x.substr(2);
  x.pop_back();
  button.x = std::stoi(x);
  y = y.substr(2);
  button.y = std::stoi(y);

  return button;
}

auto get_prize(std::istringstream &iss) -> std::optional<Coordinates<long long>> {
  std::string temp;
  std::string x;
  std::string y;
  Coordinates<long long> prize;

  //    Prize:  X=8, Y=54
  iss >> temp >> x >> y;
  if (temp != "Prize:") {
    return std::nullopt;
  }
  x = x.substr(2);
  prize.x = std::stoi(x);
  y = y.substr(2);
  prize.y = std::stoi(y);

  return prize;
}

auto get_machines(const std::vector<std::string> &input, const bool part_2) -> std::vector<Machine> {
  std::vector<Machine> machines;
  std::istringstream iss{multiline_string_to_single_line(input)};
  while (iss) {
    Machine machine;

    const auto maybe_button_a = get_button(iss);
    if (!maybe_button_a.has_value()) {
      break;
    }
    machine.button_a = maybe_button_a.value();

    const auto maybe_button_b = get_button(iss);
    if (!maybe_button_b.has_value()) {
      break;
    }
    machine.button_b = maybe_button_b.value();

    const auto maybe_prize = get_prize(iss);
    if (!maybe_prize.has_value()) {
      break;
    }
    machine.prize = maybe_prize.value();
    if (part_2) {
      machine.prize.x += unit_conversion_shift;
      machine.prize.y += unit_conversion_shift;
    }

    machines.push_back(machine);
  }
  return machines;
}

void print_machine(const Machine &machine) {
  std::cout << "Button A: " << machine.button_a.x << ", " << machine.button_a.y << '\n';
  std::cout << "Button B: " << machine.button_b.x << ", " << machine.button_b.y << '\n';
  std::cout << "Prize: " << machine.prize.x << ", " << machine.prize.y << '\n';
}
