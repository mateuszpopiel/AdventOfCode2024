# Advent of Code 2024

Welcome to the [Advent of Code 2024](https://adventofcode.com/2024) repository! This project contains my solutions for the Advent of Code 2024 challenges.

## Structure

Each day's solution is contained within its own directory under the `src` folder. The structure is as follows:

```
AdventOfCode2024/
├── 1/
│   ├── inc/
|   |    └── theme.hpp
│   ├── resources/
|   |    └── data.txt
│   ├── src/
|   |    ├── theme.cpp
|   |    └── main.cpp
│   └── tests/
|        └── test_day_1.cpp
|
...
```

- `data.txt` contains the puzzle input for the day.
- `theme.cpp` contains the C++ code to solve the puzzle. Each day has its own name so the team is included here.

## Running the Solutions

Each puzzly consists of 2 parts and for each of them there was a test written using Google Test Framework. One can run all the tests with the command.

```bash
cmake --preset Tests && cmake --build --preset Tests && ctest --preset Tests
```

To check the solution for the single day one can run the preset specified for the one one would like to check. Example for the day 1 presented below.

```bash
cmake --preset "Day 1" && cmake --build --preset "Day 1"
cd build/day_1/1 && ./day_1
```

## Requirements

- cmake 3.28 or higher
- gcc 11 or anyone supporting C++20
- GTest 1.14 or higher
- clang-tidy

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [Advent of Code](https://adventofcode.com/) by Eric Wastl

Happy coding!
