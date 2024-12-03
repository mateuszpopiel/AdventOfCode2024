#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <vector>
#include <sstream>
#include <string>

static constexpr std::string_view filename {"data.txt"};

auto open_file(const std::string_view &filename = filename) {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    return file;
}

auto convert_file_to_one_line_string(std::ifstream &file) {
    std::ostringstream buffer;
    std::string line;
    while (std::getline(file, line)) {
        buffer << line;
    }

    return buffer.str();
}

auto get_file_as_string(const std::string_view &filename) {
    auto file = open_file(filename);
    return convert_file_to_one_line_string(file);
}

void filter_string(std::string &file_as_str) {
    while (file_as_str.contains("don't")) {
        std::size_t start = file_as_str.find("don't()");
        if (start == std::string::npos) {
            break;
        }
        std::size_t end = file_as_str.find("do()", start);
        if (end == std::string::npos) {
            file_as_str = file_as_str.substr(0, start);
            break;
        }
        file_as_str = file_as_str.substr(0, start) + file_as_str.substr(end, file_as_str.size() - end);
    }
}

unsigned int mul_string_over(const std::string &line) {
    std::string line_copy = line;
    unsigned int value = 0;
    while(line_copy.contains("mul")) {
        std::size_t start = line_copy.find("mul(") + 4;
        std::size_t end = line_copy.find(")", start);
        std::string mul_content = line_copy.substr(start, end - start);
        if (std::count(mul_content.begin(), mul_content.end(), ',') != 1 || 
            !std::all_of(mul_content.begin(), mul_content.end(), [](char c) { return std::isdigit(c) || c == ' ' || c == ','; })) {
            line_copy = line_copy.substr(start);
            continue;
        }
        std::replace(mul_content.begin(), mul_content.end(), ',', ' ');

        std::istringstream iss(mul_content);
        int x, y;
        iss >> x >> y;
        value += x * y;
        line_copy = line_copy.substr(end + 1);
    }
    return value;
}

unsigned int mul_it_over(std::string &file_as_string) {
    filter_string(file_as_string);
    return mul_string_over(file_as_string);
}

int main() {
    auto file_as_str = get_file_as_string(filename);
    const auto value = mul_it_over(file_as_str);
    std::cout << "Value: " << value << std::endl;
    return 0;
}
