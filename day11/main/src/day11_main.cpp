#include <day11_solution.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <sstream>
#include <fstream>
#include <chrono>
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

constexpr int aocYear = 2024;
constexpr int aocDay = 11;

TEST_CASE("day11 - part 1") {
    SUBCASE("example 1") {
        std::istringstream iss{"0 1 10 99 999"};
        const Day11Solution::Part1ResultType ans{7};
        Day11Solution sol;
        const auto res = sol.part1(iss, 1);
        REQUIRE(res == ans);
    }
    SUBCASE("example 2") {
        for (int cnt = 1; auto ans : {3, 4, 5, 9, 13, 22}) {
            std::istringstream iss{"125 17"};
            Day11Solution sol;
            const auto res = sol.part1(iss, cnt++);
            REQUIRE(res == ans);
        }
        std::istringstream iss{"125 17"};
        Day11Solution sol;
        const auto res = sol.part1(iss, 25);
        const Day11Solution::Part1ResultType ans{55312};
        REQUIRE(res == ans);
    }
}

/*TEST_CASE("day11 - part 2") {
    std::istringstream iss{""};
    const Day11Solution::Part2ResultType ans{};
    Day11Solution sol;
    const auto res = sol.part2(iss);
    REQUIRE(res == ans);
}*/

auto main(int argc, char** argv) -> int {
    using namespace std::chrono;
    setVirtualTerminal();
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    AdventOfCodeSession session(aocYear, aocDay);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day11Solution sol;
            auto startTime = steady_clock::now();
            const auto res1 = sol.part1(ifs, 25);
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