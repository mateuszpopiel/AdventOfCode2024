#include "red-nosed_report.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using Level = int;
using Report = std::vector<Level>;

std::vector<Report> get_reports(const std::string_view &filename) {
  auto file = open_file(filename);
  auto reports = parse_file<Level>(file); // automatic move
  file.close();
  return reports;
}

bool is_increasing(const Report &report) {
  for (ull i = 1; i < report.size(); ++i) {
    if (report[i] <= report[i - 1] || std::abs(report[i] - report[i - 1]) > 3) {
      return false;
    }
  }
  return true;
}

bool is_decreasing(const Report &report) {
  for (ull i = 1; i < report.size(); ++i) {
    if (report[i] >= report[i - 1] || std::abs(report[i] - report[i - 1]) > 3) {
      return false;
    }
  }
  return true;
}

bool is_safe(const Report &report) { return is_increasing(report) || is_decreasing(report); }

bool can_be_safe_with_removal(const Report &report) {
  for (ull i = 0; i < report.size(); ++i) {
    Report modified_report = report;
    modified_report.erase(std::next(modified_report.begin(), static_cast<long int>(i)));
    if (is_safe(modified_report)) {
      return true;
    }
  }
  return false;
}

ull count_safe_reports(const std::vector<Report> &reports, const bool part_2) {
  ull safe_without_removal = 0;
  ull safe_with_removal = 0;

  for (const auto &report : reports) {
    if (is_safe(report)) {
      ++safe_without_removal;
      ++safe_with_removal; // Already safe, no need for removal
    } else if (can_be_safe_with_removal(report)) {
      ++safe_with_removal;
    }
  }
  if (!part_2) {
    return safe_without_removal;
  }
  return safe_with_removal;
}

auto solve(const bool part_2) -> ull {
  const auto reports = get_reports("data.txt");
  return count_safe_reports(reports, part_2);
}
