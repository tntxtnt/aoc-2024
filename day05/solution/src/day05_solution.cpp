#include <day05_solution.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stack>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

auto Day5Solution::part1(std::istream& is) -> Part1ResultType {
    std::unordered_map<int, std::unordered_set<int>> orderingAfter;
    char ignore;
    for (std::string line : views::istream<LineWrapper>(is)) {
        if (line.empty()) break;
        std::istringstream iss{line};
        int a;
        int b;
        iss >> a >> ignore >> b;
        orderingAfter[a].insert(b);
    }
    Part1ResultType res{};
    for (std::string line : views::istream<LineWrapper>(is)) {
        std::istringstream iss{line};
        std::vector<int> v(1);
        iss >> v[0];
        for (int n; iss >> ignore >> n;) v.push_back(n);
        if (ranges::is_sorted(v, [&](int a, int b) { return orderingAfter[a].contains(b); })) res += v[v.size() / 2];
    }
    return res;
}

auto Day5Solution::part2(std::istream& is) -> Part2ResultType {
    std::unordered_map<int, std::unordered_set<int>> orderingAfter;
    char ignore;
    for (std::string line : views::istream<LineWrapper>(is)) {
        if (line.empty()) break;
        std::istringstream iss{line};
        int a;
        int b;
        iss >> a >> ignore >> b;
        orderingAfter[a].insert(b);
    }
    auto comp = [&](int a, int b) { return orderingAfter[a].contains(b); };
    Part1ResultType res{};
    for (std::string line : views::istream<LineWrapper>(is)) {
        std::istringstream iss{line};
        std::vector<int> v(1);
        iss >> v[0];
        for (int n; iss >> ignore >> n;) v.push_back(n);
        if (!ranges::is_sorted(v, comp)) {
            ranges::sort(v, comp);
            res += v[v.size() / 2];
        }
    }
    return res;
}