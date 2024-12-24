#include <day24_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <stack>
#include <queue>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

enum class Tribool : signed char { Indetermined = -1, False, True };

template <class BinaryFunc>
auto triboolOp(Tribool lhs, Tribool rhs, BinaryFunc func) -> Tribool {
    return lhs == Tribool::Indetermined || rhs == Tribool::Indetermined ? Tribool::Indetermined
                                                                        : (Tribool)(int)func((bool)lhs, (bool)rhs);
}

struct LogicGate {
    Tribool (*oper)(Tribool lhs, Tribool rhs);
    Tribool input1{Tribool::Indetermined};
    Tribool input2{Tribool::Indetermined};
    std::string_view typeName;

    auto output() const -> Tribool { return oper(input1, input2); }
    static LogicGate AndGate() {
        return LogicGate{.oper = [](Tribool lhs, Tribool rhs) { return triboolOp(lhs, rhs, std::bit_and{}); },
                         .typeName = "AND"};
    };
    static LogicGate OrGate() {
        return LogicGate{.oper = [](Tribool lhs, Tribool rhs) { return triboolOp(lhs, rhs, std::bit_or{}); },
                         .typeName = "OR"};
    };
    static LogicGate XorGate() {
        return LogicGate{.oper = [](Tribool lhs, Tribool rhs) { return triboolOp(lhs, rhs, std::bit_xor{}); },
                         .typeName = "XOR"};
    };
    static LogicGate TrueGate() {
        return LogicGate{.oper = [](Tribool /*unused*/, Tribool /*unused*/) { return Tribool::True; },
                         .typeName = "TRUE"};
    };
    static LogicGate FalseGate() {
        return LogicGate{.oper = [](Tribool /*unused*/, Tribool /*unused*/) { return Tribool::False; },
                         .typeName = "FALSE"};
    };
};

auto Day24Solution::part1(std::istream& inputStream) -> Part1ResultType {
    std::unordered_map<std::string, LogicGate> map;
    std::unordered_map<std::string, std::vector<std::pair<std::string, bool>>> out;
    std::stack<std::string> stack;
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        if (line.empty()) break;
        std::string gateName = line.substr(0, 3);
        map[gateName] = line.back() == '1' ? LogicGate::TrueGate() : LogicGate::FalseGate();
        stack.push(gateName);
    }
    for (std::string in1Name, in2Name, opName, outName, ignore;
         inputStream >> in1Name >> opName >> in2Name >> ignore >> outName;) {
        out[in1Name].emplace_back(outName, false);
        out[in2Name].emplace_back(outName, true);
        map[outName] = opName == "OR"    ? LogicGate::OrGate()
                       : opName == "XOR" ? LogicGate::XorGate()
                                         : LogicGate::AndGate();
    }
    while (!stack.empty()) {
        const auto curr = std::move(stack.top());
        stack.pop();
        for (const auto& [next, nextIsRhs] : out[curr]) {
            (nextIsRhs ? map[next].input2 : map[next].input1) = map[curr].output();
            stack.push(next);
        }
    }
    Part1ResultType res{};
    for (auto& [gateName, gate] : map)
        if (gateName[0] == 'z') {
            const auto bitIndex = std::stoi(gateName.substr(1));
            const Part1ResultType mask = gate.output() == Tribool::True ? 1ULL << bitIndex : 0;
            res |= mask;
        }
    return res;
}

auto Day24Solution::part2(std::istream& inputStream) -> Part2ResultType {
    std::unordered_map<std::string, LogicGate> map;
    std::unordered_map<std::string, std::vector<std::pair<std::string, bool>>> out;
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        if (line.empty()) break;
        std::string gateName = line.substr(0, 3);
        map[gateName] = line.back() == '1' ? LogicGate::TrueGate() : LogicGate::FalseGate();
    }
    for (std::string in1Name, in2Name, opName, outName, ignore;
         inputStream >> in1Name >> opName >> in2Name >> ignore >> outName;) {
        out[in1Name].emplace_back(outName, false);
        out[in2Name].emplace_back(outName, true);
        map[outName] = opName == "OR"    ? LogicGate::OrGate()
                       : opName == "XOR" ? LogicGate::XorGate()
                                         : LogicGate::AndGate();
    }
    aoc::println("digraph G {");
    aoc::println("  {");
    for (auto& [key, val] : map) {
        if (key[0] == 'x' || key[0] == 'y') {
            aoc::println("    {} [style=filled fillcolor=grey shape=circle label=\"{}\"]", key, key);
        } else {
            std::string color = val.typeName == "AND" ? "yellow" : val.typeName == "OR" ? "cyan" : "pink";
            aoc::println("    g{} [style=filled fillcolor={} shape=diamond label=\"{}\"]", key, color, val.typeName); 
            if (key[0] == 'z') {
                aoc::println("    {} [style=filled fillcolor=green shape=box label=\"{}\"]", key, key);
            } else {
                aoc::println("    {} [shape=box label=\"{}\"]", key, key);
            }
        }
    }
    aoc::println("  }");
    for (auto& [key, val] : out) {
        ranges::sort(val, [](auto& kv1, auto& kv2) { return kv1.second < kv2.second; });
        for (const auto& [next, nextIsRhs] : val) {
            aoc::println("    {} -> g{}", key, next);
        }
        if (map[key].typeName != "TRUE" && map[key].typeName != "FALSE") {
            aoc::println("    g{} -> {}", key, key);
        }
    }
    for (int i = 0; i < 46; ++i) {
        std::string key = fmt::format("z{:02d}", i);
        aoc::println("    g{} -> {}", key, key);
    }
    aoc::println("}");

    // Copy digraph { ... } output to in.txt
    // Run `dot -Tpng in.txt > out.png`
    // Look for z gate that are not from XOR output (3 gates)
    // Look for X^Y and X&Y out of order (4th gate)
    /* Truth table
X Y CIN    Z COUT   X^Y X&Y  CIN^(X^Y) CIN&(X^Y) (CIN&(X^Y))|(X&Y)
0 0 0      0 0       0   0      0         0                 0
0 0 1      1 0       0   0      1         0                 0
0 1 0      1 0       1   0      1         0                 0
0 1 1      0 1       1   0      0         1                 1
1 0 0      1 0       1   0      1         0                 0
1 0 1      0 1       1   0      0         1                 1
1 1 0      0 1       0   1      0         0                 1
1 1 1      1 1       0   1      1         0                 1
                                Z                          COUT
    */
    // Search...

    return "";
}