#include <day24_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <stack>
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

using Gates = std::unordered_map<std::string, LogicGate>;
using Wirings = std::unordered_map<std::string, std::vector<std::pair<std::string, bool>>>;

auto parseGatesAndWirings(const std::vector<std::string>& lines) -> std::pair<Gates, Wirings> {
    Gates gates;
    Wirings wirings;
    for (const auto& line : lines) {
        if (line.empty()) break;
        std::string gateName = line.substr(0, 3);
        gates[gateName] = line.back() == '1' ? LogicGate::TrueGate() : LogicGate::FalseGate();
    }
    for (const auto& line : lines) {
        std::istringstream iss{line};
        std::string in1Name, in2Name, opName, outName, ignore;
        iss >> in1Name >> opName >> in2Name >> ignore >> outName;
        wirings[in1Name].emplace_back(outName, false);
        wirings[in2Name].emplace_back(outName, true);
        gates[outName] = opName == "OR"    ? LogicGate::OrGate()
                         : opName == "XOR" ? LogicGate::XorGate()
                                           : LogicGate::AndGate();
    }
    return {gates, wirings};
}

auto Day24Solution::part1(std::istream& inputStream) -> Part1ResultType {
    auto [gates, wirings] =
        parseGatesAndWirings(ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream)));
    std::stack<std::string> stack;
    for (auto& [gateName, gate] : gates)
        if (gate.typeName == "TRUE" || gate.typeName == "FALSE") stack.push(gateName);
    while (!stack.empty()) {
        const auto curr = std::move(stack.top());
        stack.pop();
        for (const auto& [next, nextIsRhs] : wirings[curr]) {
            (nextIsRhs ? gates[next].input2 : gates[next].input1) = gates[curr].output();
            if (gates[next].output() != Tribool::Indetermined) stack.push(next);
        }
    }
    Part1ResultType res{};
    for (auto& [gateName, gate] : gates)
        if (gateName[0] == 'z') {
            const auto bitIndex = std::stoi(gateName.substr(1));
            const Part1ResultType mask = gate.output() == Tribool::True ? 1ULL << bitIndex : 0;
            res |= mask;
        }
    return res;
}

/*
CIN X Y | X^Y X&Y  CIN&(X^Y) (CIN&(X^Y))|(X&Y) CIN^(X^Y)
 0  0 0 |  0   0      0                 0         0
 0  0 1 |  1   0      0                 0         1
 0  1 0 |  1   0      0                 0         1
 0  1 1 |  0   1      0                 1         0
 1  0 0 |  0   0      0                 0         1
 1  0 1 |  1   0      1                 1         0
 1  1 0 |  1   0      1                 1         0
 1  1 1 |  0   1      0                 1         1
           E   A      S               COUT        Z
                     C&E               S|A       C^E
*/
constexpr unsigned correctCarryOutValue = 0b11101000;
constexpr unsigned correctZValue = 0b10010110;

auto runAllPossibleValues(int bit, const std::string& cinLabel, std::unordered_map<std::string, LogicGate>& gates,
                          const std::unordered_map<std::string, std::vector<std::pair<std::string, bool>>>& wirings)
    -> std::unordered_map<std::string, unsigned char> {
    const std::string xLabel = fmt::format("x{:02d}", bit);
    const std::string yLabel = fmt::format("y{:02d}", bit);
    std::unordered_map<std::string, unsigned char> res;
    for (unsigned i = 0; i < 8; ++i) {
        if (res.empty()) {
            for (auto& [gateName, gate] : gates) gate.input1 = gate.input2 = Tribool::Indetermined;
        } else {
            for (auto& [gateName, outputValues] : res)
                gates[gateName].input1 = gates[gateName].input2 = Tribool::Indetermined;
        }
        gates[cinLabel] = (bool)(i & 0b100) ? LogicGate::TrueGate() : LogicGate::FalseGate();
        gates[xLabel] = (bool)(i & 0b10) ? LogicGate::TrueGate() : LogicGate::FalseGate();
        gates[yLabel] = (bool)(i & 0b1) ? LogicGate::TrueGate() : LogicGate::FalseGate();
        std::stack<std::string> stack;
        stack.push(xLabel);
        stack.push(yLabel);
        stack.push(cinLabel);
        while (!stack.empty()) {
            const auto curr = std::move(stack.top());
            stack.pop();
            if (auto it = wirings.find(curr); it != end(wirings)) {
                for (const auto& [next, nextIsRhs] : it->second) {
                    (nextIsRhs ? gates[next].input2 : gates[next].input1) = gates[curr].output();
                    if (gates[next].output() != Tribool::Indetermined) {
                        if ((bool)gates[next].output()) res[next] |= 1 << i;
                        stack.push(next);
                    }
                }
            }
        }
    }
    return res;
}

void printGraphViz(const Gates& gates, const Wirings& wirings);

auto Day24Solution::part2(std::istream& inputStream) -> Part2ResultType {
    auto lines = ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream));
    auto [gates, wirings] = parseGatesAndWirings(lines);
    std::string carry;
    for (std::string_view line : lines)
        if (line.contains("x00") && line.contains("y00") && line.contains("AND")) {
            carry = line.substr(line.size() - 3);
            break;
        }
    // std::unordered_map<std::string, std::string> newGateNames;
    std::vector<std::string> swappedGates;
    for (int i = 1; gates.contains(fmt::format("x{:02d}", i)); ++i) {
        // newGateNames[carry] = fmt::format("c{:02d}", i);
        // aoc::println("{} -> {}", carry, newGateNames[carry]);
        auto changes = runAllPossibleValues(i, carry, gates, wirings);
        const auto zLabel = fmt::format("z{:02d}", i);
        if (changes[zLabel] != correctZValue) { // incorrect wiring
            // Find incorrect pair by brute-forcing 10 choices of pairs from 5 possible gates
            std::vector<std::string> possibleGates;
            for (auto& [gateName, outputValues] : changes) possibleGates.push_back(gateName);
            for (bool found = false; auto [j, gate1Name] : views::enumerate(possibleGates)) {
                if (found) break;
                auto it1 = ranges::find_if(lines, [&](std::string_view line) { return line.ends_with(gate1Name); });
                for (auto [k, gate2Name] : views::enumerate(possibleGates) | views::drop(j + 1)) {
                    auto it2 = ranges::find_if(lines, [&](std::string_view line) { return line.ends_with(gate2Name); });
                    std::string oldLine1 = std::exchange(*it1, it1->substr(0, it1->size() - 3) + gate2Name);
                    std::string oldLine2 = std::exchange(*it2, it2->substr(0, it2->size() - 3) + gate1Name);
                    std::tie(gates, wirings) = parseGatesAndWirings(lines);
                    changes = runAllPossibleValues(i, carry, gates, wirings);
                    if (changes[zLabel] != 0b10010110) { // wrong pair, undo/swap back
                        *it1 = std::move(oldLine1);
                        *it2 = std::move(oldLine2);
                    } else {
                        // aoc::println("--> Found {} and {}", gate1Name, gate2Name);
                        swappedGates.push_back(gate1Name);
                        swappedGates.push_back(gate2Name);
                        found = true;
                        break;
                    }
                }
            }
        }
        for (auto& [gateName, outputValues] : changes)
            if (outputValues == correctCarryOutValue) {
                carry = gateName;
                break;
            }
    }
    ranges::sort(swappedGates);
    return ranges::to<std::string>(swappedGates | views::join_with(','));
}

void printGraphViz(const Gates& gates, const Wirings& wirings) {
    aoc::println("digraph G {");
    aoc::println("  {");
    for (auto& [key, val] : gates) {
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
    for (auto& [key, val] : wirings) {
        for (const auto& [next, nextIsRhs] : val) {
            aoc::println("    {} -> g{}", key, next);
        }
        if (gates.at(key).typeName != "TRUE" && gates.at(key).typeName != "FALSE") {
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
}