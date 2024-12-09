#include <day09_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
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
        for (int repeat = ch - '0'; repeat--;) blocks.push_back(val);
    }
    for (size_t i = 0, j = blocks.size() - 1; i < j;) {
        while (blocks[i] != -1 && i < j && i < blocks.size() && j >= 0) ++i;
        while (blocks[j] == -1 && i < j && i < blocks.size() && j >= 0) --j;
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
    std::vector<std::pair<int, int>> spacePockets;
    std::vector<std::pair<int, int>> filePockets;
    for (auto [i, ch] : views::enumerate(input)) {
        const int val = i % 2 == 0 ? (int)i / 2 : -1;
        (i % 2 == 0 ? filePockets : spacePockets).emplace_back((int)blocks.size(), ch - '0');
        for (int repeat = ch - '0'; repeat--;) blocks.push_back(val);
    }
    for (auto [filePocketId, filePocketLen] : views::reverse(filePockets)) {
        for (auto& [spacePocketId, spacePocketLen] : spacePockets) {
            if (spacePocketId >= filePocketId) break;
            if (spacePocketLen >= filePocketLen) {
                for (size_t i = 0; i < filePocketLen; ++i)
                    std::swap(blocks[spacePocketId++], blocks[filePocketId + i]);
                spacePocketLen -= filePocketLen;
                break;
            }
        }
    }
    Part1ResultType res{};
    for (auto [i, val] : views::enumerate(blocks)) {
        if (val == -1) continue;
        res += (long long)i * val;
    }
    return res;
}