#include "str_to_machine_helper.hpp"

long long get_minimum_tokens_to_win(const Machine &machine) {
  const long long press_a_cost = 3;
  const long long press_b_cost = 1;
  const auto press_b = ((machine.button_a.y * machine.prize.x) - (machine.button_a.x * machine.prize.y))
    / ((machine.button_a.y*machine.button_b.x) - (machine.button_a.x*machine.button_b.y)); 
  const auto press_a = (machine.prize.x - (press_b * machine.button_b.x)) / machine.button_a.x;
  if (machine.button_a * press_a + machine.button_b * press_b == machine.prize) {
    return press_a * press_a_cost + press_b * press_b_cost;
  }
  return 0;
}

auto get_minimum_tokens_to_win_all_prizes(const std::vector<Machine> &machines) -> size_t {
  size_t total_tokens = 0;
  for (const auto &machine : machines) {
    total_tokens += get_minimum_tokens_to_win(machine);
  }
  return total_tokens;
}

auto solve(const std::string_view filename, const bool part_2) -> size_t {
  const auto input = get_input_from_multiline_file(filename);
  const auto machines = get_machines(input, part_2);
  return get_minimum_tokens_to_win_all_prizes(machines);
}

int main() {
  std::cout << solve("data.txt", false) << '\n'; // 39290
  std::cout << solve("data.txt", true) << '\n'; // 73458657399094
  return 0;
}
