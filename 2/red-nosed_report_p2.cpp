#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

// Define types for clarity
using Level = int;
using Report = std::vector<Level>;

static constexpr std::string_view filename = "data.txt";

// Helper to load reports from the file
std::vector<Report> get_reports(const std::string_view &filename) {
  std::vector<Report> reports;
  std::ifstream file(filename.data());
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return reports;
  }

  std::string line;
  while (std::getline(file, line)) {
    Report report;
    std::istringstream iss(line);
    Level level;
    while (iss >> level) {
      report.push_back(level);
    }
    reports.push_back(report);
  }
  return reports;
}

bool is_increasing(const Report &report) {
  for (size_t i = 1; i < report.size(); ++i) {
    if (report[i] <= report[i - 1] || std::abs(report[i] - report[i - 1]) > 3) {
      return false;
    }
  }
  return true;
}

bool is_decreasing(const Report &report) {
  for (size_t i = 1; i < report.size(); ++i) {
    if (report[i] >= report[i - 1] || std::abs(report[i] - report[i - 1]) > 3) {
      return false;
    }
  }
  return true;
}

bool is_safe(const Report &report) { return is_increasing(report) || is_decreasing(report); }

bool can_be_safe_with_removal(const Report &report) {
  for (size_t i = 0; i < report.size(); ++i) {
    Report modified_report = report;
    modified_report.erase(modified_report.begin() + i);
    if (is_safe(modified_report)) {
      return true;
    }
  }
  return false;
}

void count_safe_reports(const std::vector<Report> &reports) {
  size_t safe_without_removal = 0;
  size_t safe_with_removal = 0;

  for (const auto &report : reports) {
    if (is_safe(report)) {
      ++safe_without_removal;
      ++safe_with_removal; // Already safe, no need for removal
    } else if (can_be_safe_with_removal(report)) {
      ++safe_with_removal;
    }
  }

  std::cout << "Safe reports without Problem Dampener: " << safe_without_removal << std::endl;
  std::cout << "Safe reports with Problem Dampener: " << safe_with_removal << std::endl;
}

// Main entry point
int main() {
  auto reports = get_reports(filename);
  count_safe_reports(reports);
  return 0;
}
