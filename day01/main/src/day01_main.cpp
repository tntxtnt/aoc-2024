#include <day01_solution.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <aoc_session.h>

TEST_CASE("day01 - part 1") {
    std::istringstream iss{R"(3   4
4   3
2   5
1   3
3   9
3   3
)"};
    const Day1Solution::Part1ResultType ans{11};
    Day1Solution sol;
    const auto res = sol.part1(iss);
    REQUIRE(res == ans);
}

TEST_CASE("day01 - part 2") {
    std::istringstream iss{R"(3   4
4   3
2   5
1   3
3   9
3   3
)"};
    const Day1Solution::Part2ResultType ans{31};
    Day1Solution sol;
    const auto res = sol.part2(iss);
    REQUIRE(res == ans);
}

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    AdventOfCodeSession session(2024, 1);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day1Solution sol;
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