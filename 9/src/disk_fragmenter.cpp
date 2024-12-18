#include "disk_fragmenter.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <vector>

auto string_to_int_vector(const std::string &str) {
  std::vector<int> numbers;
  numbers.reserve(str.length());
  for (const auto &c : str) {
    numbers.push_back(c - '0');
  }
  return numbers;
}

std::vector<int> rearrange_disk(std::vector<int> &disk_map) {
  std::vector<int> rearranged_disk;
  const auto rearranged_disk_size = static_cast<size_t>(std::accumulate(disk_map.begin(), disk_map.end(), 0));
  rearranged_disk.reserve(rearranged_disk_size);

  for (size_t in_disk_map_id = 0; in_disk_map_id < disk_map.size(); in_disk_map_id++) {
    for (int file_or_free_space_id = 0;
         file_or_free_space_id < disk_map[in_disk_map_id] && in_disk_map_id < disk_map.size();
         file_or_free_space_id++) {
      if (in_disk_map_id % 2 == 0) {
        // Push back file
        rearranged_disk.push_back(static_cast<int>(in_disk_map_id / 2));
        continue;
      }
      if (disk_map.back() != 0) {
        // Move file from the back to the free space
        rearranged_disk.push_back(static_cast<int>(disk_map.size() - 1) / 2);
        auto &back_element = disk_map.back();
        back_element--;
        continue;
      }
      // Free space left but file was fully moved -> continue with the next file by dropping last moved file and last
      // free space
      disk_map.erase(std::prev(disk_map.end(), 2), disk_map.end());
      file_or_free_space_id--; // Counter of free space from the beginning must be kept the same
    }
  }
  rearranged_disk.shrink_to_fit();
  return rearranged_disk;
}

size_t compute_checksum(const std::vector<int> &rearranged_disk) {
  size_t checksum = 0;
  for (size_t i = 0; i < rearranged_disk.size(); i++) {
    checksum += (static_cast<size_t>(rearranged_disk[i]) * i);
  }
  return checksum;
}

size_t get_fitting_file_id(const std::vector<int> &disk_map, const int free_space_length) {
  for (size_t file_id = disk_map.size() - 1; file_id > 0; file_id -= 2) {
    if (disk_map[file_id] <= free_space_length && disk_map[file_id] != 0) {
      return file_id;
    }
  }
  return disk_map.size();
}

std::vector<int> rearrange_disk_p2(std::vector<int> &disk_map) {
  std::vector<int> rearranged_disk;
  const auto rearranged_disk_size = static_cast<size_t>(std::accumulate(disk_map.begin(), disk_map.end(), 0));
  rearranged_disk.reserve(rearranged_disk_size);
  std::unordered_map<size_t, size_t> moved_files;

  for (size_t in_disk_map_id = 0; in_disk_map_id < disk_map.size(); in_disk_map_id++) {
    if (in_disk_map_id % 2 == 0) {
      if (moved_files.find(in_disk_map_id) != moved_files.end()) {
        // File was moved, so just fill with 0s
        rearranged_disk.insert(rearranged_disk.end(), moved_files[in_disk_map_id], 0);
      } else {
        rearranged_disk.insert(rearranged_disk.end(),
                               static_cast<std::vector<int>::size_type>(disk_map[in_disk_map_id]),
                               static_cast<int>(in_disk_map_id) / 2);
        // mark file as used (assign 0)
        disk_map[in_disk_map_id] = 0;
      }
      continue;
    }
    // At free space, try to move file here
    const auto fitting_file_id = get_fitting_file_id(disk_map, disk_map[in_disk_map_id]);
    if (static_cast<size_t>(fitting_file_id) == disk_map.size()) {
      // Fill with 0s so ID will increase but value won't
      rearranged_disk.insert(rearranged_disk.end(), static_cast<std::vector<int>::size_type>(disk_map[in_disk_map_id]),
                             0);
      continue;
    }
    rearranged_disk.insert(rearranged_disk.end(), static_cast<std::vector<int>::size_type>(disk_map[fitting_file_id]),
                           static_cast<int>(fitting_file_id) / 2);
    disk_map[in_disk_map_id] -= disk_map[fitting_file_id];

    // Save information that file was moved
    moved_files[fitting_file_id] = static_cast<size_t>(disk_map[fitting_file_id]);
    disk_map[fitting_file_id] = 0;

    // If there is still a free space that step shall be repeated
    if (disk_map[in_disk_map_id] != 0) {
      in_disk_map_id--;
    }
  }
  rearranged_disk.shrink_to_fit();
  return rearranged_disk;
}

std::vector<int> get_data(const std::string &filename) {
  return string_to_int_vector(file_to_string(filename).front());
}

ull solve(const bool part_2) {
  auto data = get_data("data.txt");
  const auto rearranged_disk = part_2 ? rearrange_disk_p2(data) : rearrange_disk(data);
  return compute_checksum(rearranged_disk);
}
