#include <day17_solution.h>

#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

using RegisterType = unsigned long long;
using Program = std::vector<int>;

struct Computer3Bit {
    RegisterType A{};
    RegisterType B{};
    RegisterType C{};
    RegisterType insPtr{};

    auto run(const Program& prog) -> Day17Solution::Part1ResultType {
        std::string output;
        for (insPtr = 0; insPtr < prog.size(); insPtr += 2) {
            const int instruction = prog[insPtr];
            const int operand = prog[insPtr + 1];
            const RegisterType comboValue = operand <= 3 ? operand : operand == 4 ? A : operand == 5 ? B : C;
            if (instruction == 0) {
                A >>= comboValue;
            } else if (instruction == 1) {
                B ^= operand;
            } else if (instruction == 2) {
                B = comboValue % 8;
            } else if (instruction == 3) {
                if (A != 0) insPtr = operand - 2;
            } else if (instruction == 4) {
                B ^= C;
            } else if (instruction == 5) {
                output += '0' + comboValue % 8;
                output += ',';
            } else if (instruction == 6) {
                B = A >> comboValue;
            } else {
                C = A >> comboValue;
            }
        }
        output.pop_back();
        return output;
    }
};

auto parseInput(std::istream& inputStream) -> std::pair<Computer3Bit, Program> {
    Computer3Bit computer;
    Program program;
    std::string ignore;
    inputStream >> ignore >> ignore >> computer.A;
    inputStream >> ignore >> ignore >> computer.B;
    inputStream >> ignore >> ignore >> computer.C;
    inputStream >> ignore;
    for (int n = 0; inputStream >> n; inputStream.ignore()) program.push_back(n);
    return {computer, program};
}

auto Day17Solution::part1(std::istream& inputStream) -> Part1ResultType {
    auto [computer, program] = parseInput(inputStream);
    return computer.run(program);
}

auto Day17Solution::part2(std::istream& inputStream) -> Part2ResultType {
    auto [computer, program] = parseInput(inputStream);
    Program subProgram(begin(program), end(program) - 2); // remove 3, 0
    auto eval = [&](RegisterType A) -> int {
        computer.A = A;
        return computer.run(subProgram)[0] - '0';
    };
    auto reverseA = [&](this auto&& reverseA, size_t outId, RegisterType A) -> RegisterType {
        RegisterType res{std::numeric_limits<RegisterType>::max()};
        for (RegisterType nextA : views::iota(A << 3) | views::take(8))
            if (eval(nextA) == program[outId]) res = std::min(res, outId == 0 ? nextA : reverseA(outId - 1, nextA));
        return res;
    };
    return reverseA(program.size() - 1, 0);
}