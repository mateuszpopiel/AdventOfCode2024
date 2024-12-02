#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <vector>
#include <sstream>

using Level = int;
using Report = std::vector<Level>;

static constexpr std::string_view filename {"data.txt"};

std::vector<Report> get_reports(const std::string_view &filename = filename) {
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

bool is_safe(const Report &report) {
    for(auto id = 1; id < report.size(); ++id) {
        const auto diff = std::abs(report[id] - report[id - 1]);
        if (diff < 1 || diff > 3) {
            return false;
        }
    }
    return true;
}

unsigned int count_safe_reports(const std::vector<Report> &reports) {
    unsigned int safe_levels_num = 0;
    for(const auto &report : reports) {
        if ((std::is_sorted(report.begin(), report.end()) ||
                std::is_sorted(report.begin(), report.end(), std::greater<>{})) &&
                is_safe(report)) {
            ++safe_levels_num;
        }
    };
    return safe_levels_num;
}

int main() {
    std::ifstream file(filename.data());
    const auto reports = get_reports();
    std::cout << "Safe reports: " << count_safe_reports(reports) << std::endl;
    return 0;
}
