#include <day10_solution.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <chrono>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <aoc/session.h>
#include <aoc/utils.h>

#ifdef _WIN32
#include <windows.h>
#endif

void setVirtualTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

TEST_CASE("day10 - part 1") {
    SUBCASE("example 1") {
        std::istringstream iss{R"(...0...
...1...
...2...
6543456
7.....7
8.....8
9.....9
)"};
        const Day10Solution::Part1ResultType ans{2};
        Day10Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 2") {
        std::istringstream iss{R"(..90..9
...1.98
...2..7
6543456
765.987
876....
987....
)"};
        const Day10Solution::Part1ResultType ans{4};
        Day10Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 3") {
        std::istringstream iss{R"(10..9..
2...8..
3...7..
4567654
...8..3
...9..2
.....01
)"};
        const Day10Solution::Part1ResultType ans{3};
        Day10Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 4") {
        std::istringstream iss{R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732
)"};
        const Day10Solution::Part1ResultType ans{36};
        Day10Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
}

TEST_CASE("day10 - part 2") {
    SUBCASE("example 1") {
        std::istringstream iss{R"(.....0.
..4321.
..5..2.
..6543.
..7..4.
..8765.
..9....
)"};
        const Day10Solution::Part2ResultType ans{3};
        Day10Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 2") {
        std::istringstream iss{R"(..90..9
...1.98
...2..7
6543456
765.987
876....
987....
)"};
        const Day10Solution::Part2ResultType ans{13};
        Day10Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 3") {
        std::istringstream iss{R"(012345
123456
234567
345678
4.6789
56789.
)"};
        const Day10Solution::Part2ResultType ans{227};
        Day10Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 4") {
        std::istringstream iss{R"(89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732
)"};
        const Day10Solution::Part2ResultType ans{81};
        Day10Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
}

int main(int argc, char** argv) {
    using namespace std::chrono;
    setVirtualTerminal();
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    AdventOfCodeSession session(2024, 10);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day10Solution sol;
            auto startTime = steady_clock::now();
            const auto res1 = sol.part1(ifs);
            duration<double, std::milli> elapsed = steady_clock::now() - startTime;
            session.checkAnswer(1, aoc::toString(res1));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));

            ifs.clear();
            ifs.seekg(0);
            startTime = steady_clock::now();
            const auto res2 = sol.part2(ifs);
            elapsed = steady_clock::now() - startTime;
            session.checkAnswer(2, aoc::toString(res2));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));
        } else {
            aoc::perror("Input file doesn't exists or can't be read");
            returnCode = 1;
        }
    }
    return doctestReturnCode + returnCode;
}