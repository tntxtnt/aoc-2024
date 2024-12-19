#include <day17_solution.h>
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

TEST_CASE("day17 - part 1") {
    std::istringstream iss{R"(Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0
)"};
    const Day17Solution::Part1ResultType ans{"4,6,3,5,6,3,5,2,1,0"};
    Day17Solution sol;
    const auto res = sol.part1(iss);
    REQUIRE(res == ans);
}

TEST_CASE("day17 - part 2") {
    SUBCASE("verify output itself") {
        std::istringstream iss{R"(Register A: 117440
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
)"};
        const Day17Solution::Part1ResultType ans{"0,3,5,4,3,0"};
        Day17Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("find regA value") {
        std::istringstream iss{R"(Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
)"};
        const Day17Solution::Part2ResultType ans{117440};
        Day17Solution sol;
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
    AdventOfCodeSession session(2024, 17);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day17Solution sol;
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