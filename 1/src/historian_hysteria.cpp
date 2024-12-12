#include "file_helpers.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string_view> 

using LocationId = unsigned int;

unsigned int get_total_distance(const std::pair<std::vector<LocationId>, std::vector<LocationId>> &lists_of_locations) {
    if (lists_of_locations.first.size() != lists_of_locations.second.size()) {
        throw std::invalid_argument("Lists must have the same size");
    }

    auto left_list = lists_of_locations.first;
    auto right_list = lists_of_locations.second;

    std::sort(left_list.begin(), left_list.end());
    std::sort(right_list.begin(), right_list.end());

    unsigned int distance = 0;
    for (size_t i = 0; i < left_list.size(); ++i) {
        distance += std::abs(static_cast<int>(left_list[i]) - static_cast<int>(right_list[i]));
    }
    return distance;
}

unsigned int solve() {
    auto file = open_file();

    std::vector<LocationId> left_list;
    std::vector<LocationId> right_list;

    LocationId left, right;
    while (file >> left >> right) {
        left_list.push_back(left);
        right_list.push_back(right);
    }

    return get_total_distance(std::make_pair(left_list, right_list));
}
