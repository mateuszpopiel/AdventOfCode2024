# Advent of Code 2024

Welcome to the [Advent of Code 2024](https://adventofcode.com/2024) repository! This project contains my solutions for the Advent of Code 2024 challenges.

## Structure

Each day's solution is contained within its own directory under the `src` folder. The structure is as follows:

```plaintext
AdventOfCode2024/
├── .github/
├── 0/
├── 1/
├── 2/
...
├── cmake/
│   ├── day_setup_file.cmake
│   └── ...
├── lib/
│   ├── inc/
│   └── src/
├── .clang-format
├── .clang-tidy
├── CMakeLists.txt
└── CMakePresets.json
...
```

- `.github` directory cantains .yml file specifing the CI workflow. For now it is very simle so it requires only that the test build compiles and completes. The clang-tidy is used there so it requires having a brief look on the uploaded solution.
- `day_setup_file.cmake` contains a simple cmake function that determines how the files of the day are included in the project.
- `CMakePresets.json` specifies all the configurations used.

## Quick start with a new puzzle

To not waste any moment the `setup_day.sh` bash script was written. It prepares a day specific directory with all the files already there!
One just needs to run `bash setup_say.sh day_nr day_theme and the directories will be filled as in the example:

```plaintext
AdventOfCode2024/
├── day_nr/
│   ├── inc/
|   |    └── day_theme.hpp
│   ├── resources/
|   |    └── data.txt
│   ├── src/
|   |    ├── day_theme.cpp
|   |    └── main.cpp
│   └── tests/
|        └── test_day_day_nr.cpp
|
...
```

Example code is there as well so it is ready to run and be replaced with an actual puzzle solution

## Running the Solutions

Each puzzly consists of 2 parts and for each of them there was a test written using Google Test Framework. One can run all the tests with the command.

```bash
cmake --preset Tests && cmake --build --preset Tests && ctest --preset Tests
```

To check the solution for the single day one can first run the preset **"All days"**. It builds binaries for all the solutions. Then one shall go to the binary directory and run it. The `data.txt` file will be already there. The binary simply writes the solution into the standard output. Example for day 1 is presented below.

```bash
cmake --preset "Day 1" && cmake --build --preset "Day 1"
cd build/all_days/1 && ./day_1
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
