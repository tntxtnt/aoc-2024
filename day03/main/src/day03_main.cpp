#include <day03_solution.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <aoc_session.h>

TEST_CASE("day03 - part 1") {
    std::istringstream iss{R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
)"};
    const Day3Solution::Part1ResultType ans{161};
    Day3Solution sol;
    const auto res = sol.part1(iss);
    REQUIRE(res == ans);
}

TEST_CASE("day03 - part 2") {
    SUBCASE("with don't") {
        std::istringstream iss{R"(xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
    )"};
        const Day3Solution::Part2ResultType ans{48};
        Day3Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("without don't") {
        std::istringstream iss{R"(xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
    )"};
        const Day3Solution::Part2ResultType ans{161};
        Day3Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
}

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    AdventOfCodeSession session(2024, 3);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day3Solution sol;
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
    } else {
        fmt::println(std::cerr, "{}", fmt::styled("Cannot fetch input file", fmt::fg(fmt::color::red)));
        returnCode = 1;
    }
    return doctestReturnCode + returnCode;
}