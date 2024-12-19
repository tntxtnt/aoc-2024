#include <day01_solution.h>
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
    const Day1Solution::Part2ResultType ans{};
    Day1Solution sol;
    const auto res = sol.part2(iss);
    REQUIRE(res == ans);
}

int main(int argc, char** argv) {
    using namespace std::chrono;
    setVirtualTerminal();
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    const int doctestReturnCode = context.run();
    if (context.shouldExit() || doctestReturnCode != EXIT_SUCCESS) return doctestReturnCode;

    int returnCode = 0;
    AdventOfCodeSession session(2024, 1);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day1Solution sol;
            auto startTime = steady_clock::now();
            const auto res1 = sol.part1(ifs);
            duration<double, std::milli> elapsed = steady_clock::now() - startTime;
            session.checkAnswer(1, aoc::toString(res1));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));

            /*ifs.clear();
            ifs.seekg(0);
            startTime = steady_clock::now();
            const auto res2 = sol.part2(ifs);
            elapsed = steady_clock::now() - startTime;
            session.checkAnswer(2, aoc::toString(res2));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));*/
        } else {
            aoc::perror("Input file doesn't exists or can't be read");
            returnCode = 1;
        }
    }
    return doctestReturnCode + returnCode;
}