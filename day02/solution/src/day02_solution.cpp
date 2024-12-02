#include "day02_solution.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;

static auto isSafe(const std::vector<int>& v) -> bool {
    if (ranges::is_sorted(v)) {
        return ranges::all_of(
            views::adjacent<2>(v), [](int diff) { return 0 < diff && diff <= 3; },
            [](const auto& p) { return std::get<1>(p) - std::get<0>(p); });
    } else if (ranges::is_sorted(v, std::greater<int>{})) {
        return ranges::all_of(
            views::adjacent<2>(v), [](int diff) { return 0 < diff && diff <= 3; },
            [](const auto& p) { return std::get<0>(p) - std::get<1>(p); });
    }
    return false;
}

auto Day2Solution::part1(std::istream& is) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string line; std::getline(is, line);) {
        std::vector<int> v;
        std::istringstream iss{line};
        for (int n; iss >> n;) v.push_back(n);
        res += (int)isSafe(v);
    }
    return res;
}

auto Day2Solution::part2(std::istream& is) -> Part2ResultType {
    Part2ResultType res{};
    for (std::string line; std::getline(is, line);) {
        std::vector<int> v;
        std::istringstream iss{line};
        for (int n; iss >> n;) v.push_back(n);
        if (isSafe(v)) {
            ++res;
        } else {
            for (size_t i = 0; i < v.size(); ++i) {
                auto u = v;
                u.erase(begin(u) + i);
                if (isSafe(u)) {
                    ++res;
                    break;
                }
            }
        }
    }
    return res;
}