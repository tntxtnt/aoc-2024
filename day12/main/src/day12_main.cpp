#include <day12_solution.h>
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

TEST_CASE("day12 - part 1") {
    SUBCASE("example 1") {
        std::istringstream iss{R"(AAAA
BBCD
BBCC
EEEC
)"};
        const Day12Solution::Part1ResultType ans{140};
        Day12Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 2") {
        std::istringstream iss{R"(OOOOO
OXOXO
OOOOO
OXOXO
OOOOO
)"};
        const Day12Solution::Part1ResultType ans{772};
        Day12Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 3") {
        std::istringstream iss{R"(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE
)"};
        const Day12Solution::Part1ResultType ans{1930};
        Day12Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
}

TEST_CASE("day12 - part 2") {
    SUBCASE("example 1") {
        std::istringstream iss{R"(AAAA
BBCD
BBCC
EEEC
)"};
        const Day12Solution::Part1ResultType ans{80};
        Day12Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 2") {
        std::istringstream iss{R"(OOOOO
OXOXO
OOOOO
OXOXO
OOOOO
)"};
        const Day12Solution::Part1ResultType ans{436};
        Day12Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 3") {
        std::istringstream iss{R"(EEEEE
EXXXX
EEEEE
EXXXX
EEEEE
)"};
        const Day12Solution::Part1ResultType ans{236};
        Day12Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 4") {
        std::istringstream iss{R"(AAAAAA
AAABBA
AAABBA
ABBAAA
ABBAAA
AAAAAA
)"};
        const Day12Solution::Part1ResultType ans{368};
        Day12Solution sol;
        const auto res = sol.part2(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("example 5") {
        std::istringstream iss{R"(RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE
)"};
        const Day12Solution::Part1ResultType ans{1206};
        Day12Solution sol;
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
    AdventOfCodeSession session(2024, 12);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day12Solution sol;
            auto startTime = steady_clock::now();
            const auto res1 = sol.part1(ifs);
            duration<double, std::milli> elapsed = steady_clock::now() - startTime;
            session.checkAnswer(1, std::to_string(res1));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));

            ifs.clear();
            ifs.seekg(0);
            startTime = steady_clock::now();
            const auto res2 = sol.part2(ifs);
            elapsed = steady_clock::now() - startTime;
            session.checkAnswer(2, std::to_string(res2));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));
        } else {
            aoc::perror("Input file doesn't exists or can't be read");
            returnCode = 1;
        }
    }
    return doctestReturnCode + returnCode;
}