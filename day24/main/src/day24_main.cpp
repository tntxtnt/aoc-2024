#include <day24_solution.h>
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

TEST_CASE("day24 - part 1") {
    SUBCASE("part 1 example 1") {
        std::istringstream iss{R"(x00: 1
x01: 1
x02: 1
y00: 0
y01: 1
y02: 0

x00 AND y00 -> z00
x01 XOR y01 -> z01
x02 OR y02 -> z02
)"};
        const Day24Solution::Part1ResultType ans{4};
        Day24Solution sol;
        const auto res = sol.part1(iss);
        REQUIRE(res == ans);
    }
    SUBCASE("part 1 example 2") {
        std::istringstream iss{R"(x00: 1
x01: 0
x02: 1
x03: 1
x04: 0
y00: 1
y01: 1
y02: 1
y03: 1
y04: 1

ntg XOR fgs -> mjb
y02 OR x01 -> tnw
kwq OR kpj -> z05
x00 OR x03 -> fst
tgd XOR rvg -> z01
vdt OR tnw -> bfw
bfw AND frj -> z10
ffh OR nrd -> bqk
y00 AND y03 -> djm
y03 OR y00 -> psh
bqk OR frj -> z08
tnw OR fst -> frj
gnj AND tgd -> z11
bfw XOR mjb -> z00
x03 OR x00 -> vdt
gnj AND wpb -> z02
x04 AND y00 -> kjc
djm OR pbm -> qhw
nrd AND vdt -> hwm
kjc AND fst -> rvg
y04 OR y02 -> fgs
y01 AND x02 -> pbm
ntg OR kjc -> kwq
psh XOR fgs -> tgd
qhw XOR tgd -> z09
pbm OR djm -> kpj
x03 XOR y03 -> ffh
x00 XOR y04 -> ntg
bfw OR bqk -> z06
nrd XOR fgs -> wpb
frj XOR qhw -> z04
bqk OR frj -> z07
y03 OR x01 -> nrd
hwm AND bqk -> z03
tgd XOR rvg -> z12
tnw OR pbm -> gnj
)"};
        const Day24Solution::Part1ResultType ans{2024};
        Day24Solution sol;
        const auto res = sol.part1(iss);
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
    AdventOfCodeSession session(2024, 24);
    if (session.fetchInput()) {
        if (std::ifstream ifs{session.inputFileName}) {
            Day24Solution sol;
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
            // session.checkAnswer(2, aoc::toString(res2));
            aoc::println("Solve time: {}", fmt::styled(elapsed, fg(fmt::color::spring_green)));
        } else {
            aoc::perror("Input file doesn't exists or can't be read");
            returnCode = 1;
        }
    }
    return doctestReturnCode + returnCode;
}