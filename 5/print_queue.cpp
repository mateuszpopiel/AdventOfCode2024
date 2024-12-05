#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <vector>
#include <sstream>
#include <array>
#include <string>
#include <utility>

static constexpr std::string_view filename {"data.txt"};

using Rules = std::vector<std::pair<unsigned int, unsigned int>>;
using Page = std::vector<unsigned int>;

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
      unsigned int first, second;
      iss >> first >> second;
      rules.push_back({first, second});
    }
  }
  return rules;
}

auto get_pages(const std::vector<std::string> &file_as_str, const size_t line_to_start) {
  auto pages_str = std::vector<std::string>(file_as_str.begin() + line_to_start, file_as_str.end());
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

auto get_numbers_preciding_number(const Rules &rules, const unsigned int number) {
  std::vector<unsigned int> numbers_before_the_number;
  for (const auto &rule : rules) {
    if (rule.second == number) {
      numbers_before_the_number.push_back(rule.first);
    }
  }
  return numbers_before_the_number;
}

auto is_preciding_num_in_subpage(const Rules &rules, const Page subpage, const unsigned int number) {
  const auto numbers_preciding_number = get_numbers_preciding_number(rules, number);
  for (const auto &number_before : numbers_preciding_number) {
    if (std::find(subpage.begin(), subpage.end(), number_before) != subpage.end()) {
      return true;
    }
  }
  return false;
}

auto is_page_in_correct_order(const Page &page, const Rules &rules) {
  for (auto page_iter = page.begin(); page_iter != page.end(); ++page_iter) {             
    if (std::next(page_iter) != page.end() &&
      is_preciding_num_in_subpage(rules, Page(std::next(page_iter), page.end()), *page_iter)) {
        std::cout << "Found a number that has to be before " << *page_iter << '\n';
      return false;
    }
  }
  return true;
}

auto get_sum_of_nums_in_the_middle_of_valid_pages(const std::vector<Page> &pages, const Rules &rules) {
  unsigned int sum = 0;
  for (const auto &page : pages) {
    if (!is_page_in_correct_order(page, rules)) {
      continue;
    }
    sum += page[page.size() / 2];
  }
  return sum;
}

int main() {
  auto file = open_file(filename);
  const auto file_as_str = file_to_string(file);
  const auto rules = get_rules(file_as_str);
  const auto pages = get_pages(file_as_str, rules.size() + 1); // There is empty line in file
  const auto sum = get_sum_of_nums_in_the_middle_of_valid_pages(pages, rules);
  std::cout << "Sum: " << sum << std::endl;

  return 0;
}
