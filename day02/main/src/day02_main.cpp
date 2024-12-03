#include <day02_solution.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <aoc_session.h>

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
    AdventOfCodeSession session(2024, 2);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day2Solution sol;
            const auto res1 = sol.part1(ifs);
            session.checkAnswer(1, std::to_string(res1));

            ifs.clear();
            ifs.seekg(0);
            const auto res2 = sol.part2(ifs);
            session.checkAnswer(2, std::to_string(res2));
        } else {
            fmt::println(std::cerr, "{}",
                         fmt::styled("Input file doesn't exists or can't be read", fmt::fg(fmt::color::red)));
            returnCode = 1;
        }
    }
    return doctestReturnCode + returnCode;
}