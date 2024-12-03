#include <day03_solution.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <ranges>
namespace ranges = std::ranges;
#include <cctype>

auto isDigit(unsigned char ch) -> bool {
    return std::isdigit(ch);
}

static auto doMul(std::string_view sv) -> int {
    constexpr std::string_view kPrefix = "mul(";
    int res{};
    for (size_t i = sv.find(kPrefix); i != sv.npos; i = sv.find(kPrefix, i)) {
        i += kPrefix.size();
        size_t j = sv.find(',', i);
        size_t k = sv.find(')', j + 1);
        auto arg1 = sv.substr(i, j - i);
        auto arg2 = sv.substr(j + 1, k - j - 1);
        if (ranges::all_of(arg1, isDigit) && ranges::all_of(arg2, isDigit))
            res += std::stoi(std::string{arg1}) * std::stoi(std::string{arg2});
    }
    return res;
}

auto Day3Solution::part1(std::istream& is) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string line; std::getline(is, line);) res += doMul(line);
    return res;
}

auto Day3Solution::part2(std::istream& is) -> Part2ResultType {
    Part2ResultType res{};
    constexpr std::string_view kEnable = "do()";
    constexpr std::string_view kDisable = "don't()";
    bool enabled = true;
    for (std::string line; std::getline(is, line);) {
        for (std::string_view sv = line; !sv.empty();) {
            if (enabled) {
                size_t iDisable = sv.find(kDisable);
                res += doMul(sv.substr(0, iDisable));
                enabled = iDisable == sv.npos;
                sv = enabled ? "" : sv.substr(iDisable + kDisable.size());
            } else {
                size_t iEnable = sv.find(kEnable);
                enabled = iEnable != sv.npos;
                sv = enabled ? sv.substr(iEnable + kEnable.size()) : "";
            }
        }
    }
    return res;
}