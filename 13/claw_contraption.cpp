#include "str_to_machine_helper.hpp"
#include <limits>

auto get_minimum_tokens_to_win(const Machine &machine) -> std::optional<size_t> {
  // A button: 3 tokens, B button: 1 token
  // Prize = 3 * X * button_a + Y * button_b
  static constexpr size_t cost_of_a = 3;
  static constexpr size_t cost_of_b = 1;
  size_t min_tokens = std::numeric_limits<size_t>::max();

  for (int button_a_press = 0; button_a_press <= 100; ++button_a_press) {
    for (int button_b_press = 0; button_b_press <= 100; ++button_b_press) {
      if ((machine.button_a * button_a_press) + (machine.button_b * button_b_press) == machine.prize) {
        size_t cost = cost_of_a * button_a_press + cost_of_b * button_b_press;
        min_tokens = std::min(min_tokens, cost);
      }
    }
  }
  if (min_tokens == std::numeric_limits<size_t>::max()) {
    return std::nullopt;
  }
  return min_tokens;
}

auto get_minimum_tokens_to_win_all_prizes(const std::vector<Machine> &machines) -> size_t {
  size_t total_tokens = 0;
  for (const auto &machine : machines) {
    const auto min_tokens = get_minimum_tokens_to_win(machine);
    if (min_tokens.has_value()) {
      total_tokens += min_tokens.value();
    }
  }
  return total_tokens;
}

auto solve_part_1(const std::string_view filename) -> size_t {
  const auto input = get_input_from_multiline_file(filename);
  const auto machines = get_machines(input);
  return get_minimum_tokens_to_win_all_prizes(machines);
}

int main() {
  std::cout << solve_part_1("data.txt") << '\n';
  return 0;
}
