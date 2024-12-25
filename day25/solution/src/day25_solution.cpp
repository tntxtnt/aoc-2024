#include <day25_solution.h>

#include <vector>
#include <string>
#include <array>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

using KeyOrLock = std::array<int, 5>;
using KeyOrLockDisplay = std::array<std::string, 7>;

auto Day25Solution::part1(std::istream& inputStream) -> Part1ResultType {
    std::vector<KeyOrLock> locks;
    std::vector<KeyOrLock> keys;
    KeyOrLockDisplay disp;
    while (inputStream >> disp[0] >> disp[1] >> disp[2] >> disp[3] >> disp[4] >> disp[5] >> disp[6]) {
        KeyOrLock keyOrLock{};
        for (size_t i = 0; i < 5; ++i)
            keyOrLock[i] = (int)ranges::count_if(disp, [&](std::string_view line) { return line[i] == '#'; }) - 1;
        (disp[0][0] == '#' ? locks : keys).push_back(keyOrLock);
    }
    Part1ResultType res{};
    for (const auto& lock : locks)
        for (const auto& key : keys)
            res += (int)ranges::all_of(views::zip(lock, key),
                                       [](const auto& tup) { return std::get<0>(tup) + std::get<1>(tup) <= 5; });
    return res;
}