#include "day02_solution.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <fmt/ostream.h>
#include <fmt/color.h>

TEST_CASE("day02 - part 1") {
    std::istringstream iss{R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
)"};
    const Day2Solution::Part1ResultType ans{2};
    Day2Solution sol;
    const auto res = sol.part1(iss);
    REQUIRE(res == ans);
}

TEST_CASE("day02 - part 2") {
    std::istringstream iss{R"(7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
)"};
    const Day2Solution::Part2ResultType ans{4};
    Day2Solution sol;
    const auto res = sol.part2(iss);
    REQUIRE(res == ans);
}

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    if (std::ifstream ifs{"day02.txt"}) {
        Day2Solution sol;
        const auto res1 = sol.part1(ifs);
        fmt::println("day02 - part 1: {}", fmt::styled(res1, fmt::fg(fmt::color::green)));

        ifs.clear();
        ifs.seekg(0);
        const auto res2 = sol.part2(ifs);
        fmt::println("day02 - part 1: {}", fmt::styled(res2, fmt::fg(fmt::color::green)));
    } else {
        fmt::println(std::cerr, "{}",
                     fmt::styled("Input file doesn't exists or can't be read", fmt::fg(fmt::color::red)));
        returnCode = 1;
    }
    return doctestReturnCode + returnCode;
}