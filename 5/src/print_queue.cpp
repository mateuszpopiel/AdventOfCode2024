#include "print_queue.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <array>
#include <string>
#include <utility>
#include <vector>

using Rules = std::vector<std::pair<ull, ull>>;
using Page = std::vector<ull>;

auto multi_line_string_to_one_line(const std::vector<std::string> &lines) {
  std::string one_line;
  for (const auto &line : lines) {
    one_line += line;
  }
  return one_line;
}

auto get_rules(const std::vector<std::string> &file_as_str) {
  Rules rules;
  for (const auto &line : file_as_str) {
    if (line.find('|') != std::string::npos) {
      auto mutable_line = line;
      std::replace(mutable_line.begin(), mutable_line.end(), '|', ' ');
      std::istringstream iss(mutable_line);
      ull first = 0;
      ull second = 0;
      iss >> first >> second;
      rules.emplace_back(std::make_pair(first, second));
    }
  }
  return rules;
}

auto get_pages(const std::vector<std::string> &file_as_str, const size_t line_to_start) {
  auto pages_str = std::vector<std::string>(
      std::next(file_as_str.begin(), static_cast<std::vector<std::string>::difference_type>(line_to_start)),
      file_as_str.end());
  std::vector<Page> pages;
  for (const auto &page_str : pages_str) {
    Page page;
    std::istringstream iss(page_str);
    std::string number;
    while (std::getline(iss, number, ',')) {
      page.push_back(std::stoul(number));
    }
    pages.push_back(page);
  }
  return pages;
}

auto get_numbers_preciding_number(const Rules &rules, const ull number) {
  std::vector<ull> numbers_before_the_number;
  for (const auto &rule : rules) {
    if (rule.second == number) {
      numbers_before_the_number.push_back(rule.first);
    }
  }
  return numbers_before_the_number;
}

auto is_preciding_num_in_subpage(const Rules &rules, const Page &subpage, const ull number) {
  const auto numbers_preciding_number = get_numbers_preciding_number(rules, number);
  return std::any_of(subpage.begin(), subpage.end(), [&numbers_preciding_number](const auto &number_before) {
    return std::find(numbers_preciding_number.begin(), numbers_preciding_number.end(), number_before) !=
           numbers_preciding_number.end();
  });
}

auto is_page_in_correct_order(const Page &page, const Rules &rules) {
  for (auto page_iter = page.begin(); page_iter != page.end(); ++page_iter) {
    if (std::next(page_iter) != page.end() &&
        is_preciding_num_in_subpage(rules, Page(std::next(page_iter), page.end()), *page_iter)) {
      return false;
    }
  }
  return true;
}

auto get_sum_of_nums_in_the_middle_of_valid_pages(const std::vector<Page> &pages, const Rules &rules) {
  ull sum = 0;
  for (const auto &page : pages) {
    if (!is_page_in_correct_order(page, rules)) {
      continue;
    }
    sum += page[page.size() / 2];
  }
  return sum;
}

auto get_number_violating_rules_iter(const Page &page, const Rules &rules, const Page::const_iterator &part_iter) {
  const auto numbers_preciding_number = get_numbers_preciding_number(rules, *part_iter);
  for (const auto &number : numbers_preciding_number) {
    const auto num_violating_rules_iter = std::find(std::next(part_iter), page.end(), number);
    if (num_violating_rules_iter != page.end()) {
      return num_violating_rules_iter;
    }
  }
  throw std::runtime_error("No number violating rules found");
}

void fix_page_one_part_in_page(Page &page, const Rules &rules) {
  for (auto page_iter = page.begin(); page_iter != page.end(); ++page_iter) {
    if (std::next(page_iter) != page.end() &&
        is_preciding_num_in_subpage(rules, Page(std::next(page_iter), page.end()), *page_iter)) {
      auto num_violating_rules_iter = get_number_violating_rules_iter(page, rules, page_iter);
      const auto num_violating_rules = *num_violating_rules_iter;
      page.erase(num_violating_rules_iter);
      page.insert(page_iter, num_violating_rules);
    }
  }
}

void fix_page(Page &page, const Rules &rules) {
  while (!is_page_in_correct_order(page, rules)) {
    fix_page_one_part_in_page(page, rules);
  }
}

auto fix_pages(const std::vector<Page> &pages, const Rules &rules) {
  std::vector<Page> fixed_pages;
  for (const auto &page : pages) {
    if (!is_page_in_correct_order(page, rules)) {
      auto page_to_fix = page;
      fix_page(page_to_fix, rules);
      fixed_pages.push_back(page_to_fix);
    }
  }
  return fixed_pages;
}

ull solve(const bool part_2) {
  const auto file_as_str = file_to_string("data.txt");
  const auto rules = get_rules(file_as_str);
  auto pages = get_pages(file_as_str, rules.size() + 1); // There is empty line in file
  if (part_2) {
    pages = fix_pages(pages, rules);
  }
  return get_sum_of_nums_in_the_middle_of_valid_pages(pages, rules);
}
