#include <day01_solution.h>
#include <vector>
#include <algorithm>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <cstdlib>
#include <map>
#include <aoc/utils.h>

auto Day1Solution::part1(std::istream& is) -> Part1ResultType {
    std::vector<int> v1;
    std::vector<int> v2;
    for (auto [n1, n2] : views::istream<Pair<int, int>>(is)) {
        v1.push_back(n1);
        v2.push_back(n2);
    }
    ranges::sort(v1);
    ranges::sort(v2);
    Part1ResultType res{};
    for (auto [n1, n2] : views::zip(v1, v2)) res += std::abs(n1 - n2);
    return res;
}

auto Day1Solution::part2(std::istream& is) -> Part2ResultType {
    std::vector<int> v1;
    std::map<int, int> m2;
    for (auto [n1, n2] : views::istream<Pair<int, int>>(is)) {
        v1.push_back(n1);
        ++m2[n2];
    }
    Part2ResultType res{};
    for (int n : v1) res += n * (m2.contains(n) ? m2[n] : 0);
    return res;
}