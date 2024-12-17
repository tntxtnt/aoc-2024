#include <day17_solution.h>

#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>
#include <fmt/ranges.h>

struct Program {
    std::vector<int> instructions;
};

struct Computer3Bit {
    using RegisterType = long long;
    RegisterType A{};
    RegisterType B{};
    RegisterType C{};
    int insPtr{};

    auto run(const Program& prog) -> Day17Solution::Part1ResultType {
        std::string output;
        for (insPtr = 0; insPtr < std::ssize(prog.instructions); insPtr += 2) {
            const int instruction = prog.instructions[insPtr];
            const int operand = prog.instructions[insPtr + 1];
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

auto Day17Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Computer3Bit computer;
    Program program;
    std::string ignore;
    inputStream >> ignore >> ignore >> computer.A;
    inputStream >> ignore >> ignore >> computer.B;
    inputStream >> ignore >> ignore >> computer.C;
    inputStream >> ignore;
    for (int n = 0; inputStream >> n; inputStream.ignore()) program.instructions.push_back(n);

    return computer.run(program);
}

auto Day17Solution::part2(std::istream& inputStream) -> Part2ResultType {
    Computer3Bit computer;
    Program program;
    std::string ignore;
    inputStream >> ignore >> ignore >> computer.A;
    inputStream >> ignore >> ignore >> computer.B;
    inputStream >> ignore >> ignore >> computer.C;
    inputStream >> ignore;
    for (int n = 0; inputStream >> n; inputStream.ignore()) program.instructions.push_back(n);

    Program subProgram = program;
    subProgram.instructions.pop_back(); // remove 0
    subProgram.instructions.pop_back(); // remove 3
    auto reverseA = [&](this auto&& reverseA, int outId, long long a) -> long long {
        if (outId == -1) return a;
        long long res{std::numeric_limits<long long>::max()};
        for (int cba = 0; cba < 8; ++cba) {
            long long nextA = (a << 3) | cba;
            computer.A = nextA;
            const int out = computer.run(subProgram)[0] - '0';
            if (out != program.instructions[outId]) continue;
            res = std::min(res, reverseA(outId - 1, nextA));
        }
        return res;
    };
    return reverseA((int)program.instructions.size() - 1, 0);
}