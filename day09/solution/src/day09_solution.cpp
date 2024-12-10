#include <day09_solution.h>

#include <vector>
#include <string>
#include <array>
#include <set>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>
#include <fmt/ranges.h>

auto Day9Solution::part1(std::istream& is) -> Part1ResultType {
    std::string input;
    is >> input;
    std::vector<int> blocks;
    for (auto [i, ch] : views::enumerate(input)) {
        const int val = i % 2 == 0 ? (int)i / 2 : -1;
        blocks.insert(end(blocks), ch - '0', val);
    }
    for (int i = 0, j = (int)blocks.size() - 1; i < j;) {
        while (blocks[i] != -1 && i < j) ++i;
        while (blocks[j] == -1 && i < j) --j;
        while (blocks[i] == -1 && blocks[j] != -1 && i < j) std::swap(blocks[i++], blocks[j--]);
    }
    Part1ResultType res{};
    for (auto [i, val] : views::enumerate(blocks)) {
        if (val == -1) break;
        res += (long long)i * val;
    }
    return res;
}

auto Day9Solution::part2(std::istream& is) -> Part2ResultType {
    std::string input;
    is >> input;
    std::vector<int> blocks;
    std::array<std::set<int>, 10> spacePockets;
    std::vector<std::pair<int, int>> filePockets;
    for (auto [i, ch] : views::enumerate(input)) {
        const int val = i % 2 == 0 ? (int)i / 2 : -1;
        if (i % 2 == 0) {
            filePockets.emplace_back((int)blocks.size(), ch - '0');
        } else if (ch != '0') {
            spacePockets[ch - '0'].insert((int)blocks.size());
        }
        blocks.insert(end(blocks), ch - '0', val);
    }
    for (auto [filePocketId, filePocketLen] : views::reverse(filePockets)) {
        int spacePocketId{-1};
        int spacePocketLen{-1};
        for (size_t len = filePocketLen; len < spacePockets.size(); ++len) {
            if (spacePockets[len].empty()) continue;
            const int id = *begin(spacePockets[len]);
            if (id >= filePocketId) continue;
            if (spacePocketId == -1 || id < spacePocketId) {
                spacePocketId = id;
                spacePocketLen = (int)len;
            }
        }
        if (spacePocketId == -1) continue;
        spacePockets[spacePocketLen].erase(spacePocketId);
        for (int i = 0; i < filePocketLen; ++i) std::swap(blocks[spacePocketId++], blocks[filePocketId + i]);
        spacePocketLen -= filePocketLen;
        if (spacePocketLen > 0) spacePockets[spacePocketLen].insert(spacePocketId);
    }
    Part1ResultType res{};
    for (auto [i, val] : views::enumerate(blocks)) {
        if (val == -1) continue;
        res += (long long)i * val;
    }
    return res;
}