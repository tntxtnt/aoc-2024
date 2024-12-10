#include <day10_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>
#include <fmt/ranges.h>

struct Coord2 {
    int r{-1};
    int c{-1};
};

auto Day10Solution::part1(std::istream& is) -> Part1ResultType {
    auto mat = ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(is));
    auto isValidCoord = [&](const Coord2& coord) {
        return 0 <= coord.r && coord.r < (int)mat.size() && 0 <= coord.c && coord.c < (int)mat[0].size();
    };
    std::vector<bool> visited(mat.size() * mat[0].size());
    auto dfs = [&](this auto&& dfs, const Coord2& source, int prevHeight) {
        if (!isValidCoord(source)) return 0;
        const int cols = (int)mat[0].size();
        if (visited[source.r * cols + source.c]) return 0;
        const auto height = mat[source.r][source.c] - '0';
        const auto diffHeight = height - prevHeight;
        if (diffHeight != 1) return 0;
        visited[source.r * cols + source.c] = true;
        if (height == 9) return 1;
        return dfs({source.r - 1, source.c}, height) + dfs({source.r + 1, source.c}, height) +
               dfs({source.r, source.c - 1}, height) + dfs({source.r, source.c + 1}, height);
    };
    Part1ResultType res{};
    for (auto [r, row] : views::enumerate(mat)) {
        for (auto [c, ch] : views::enumerate(row)) {
            ranges::fill(visited, false);
            if (ch == '0') res += dfs({(int)r, (int)c}, -1);
        }
    }
    return res;
}

auto Day10Solution::part2(std::istream& is) -> Part2ResultType {
    auto mat = ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(is));
    auto isValidCoord = [&](const Coord2& coord) {
        return 0 <= coord.r && coord.r < (int)mat.size() && 0 <= coord.c && coord.c < (int)mat[0].size();
    };
    auto dfs = [&](this auto&& dfs, const Coord2& source, int prevHeight) {
        if (!isValidCoord(source)) return 0;
        const auto height = mat[source.r][source.c] - '0';
        const auto diffHeight = height - prevHeight;
        if (diffHeight != 1) return 0;
        if (height == 9) return 1;
        return dfs({source.r - 1, source.c}, height) + dfs({source.r + 1, source.c}, height) +
               dfs({source.r, source.c - 1}, height) + dfs({source.r, source.c + 1}, height);
    };
    Part1ResultType res{};
    for (auto [r, row] : views::enumerate(mat))
        for (auto [c, ch] : views::enumerate(row))
            if (ch == '0') res += dfs({(int)r, (int)c}, -1);
    return res;
}