#include <day11_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

constexpr int aocYear = 2024;
constexpr int part2BlinkCount = 75;
constexpr int base10 = 10;

auto Day11Solution::part1(std::istream& inputStream, int blinkCount) -> Part1ResultType {
    auto vec = ranges::to<std::vector<long long>>(views::istream<long long>(inputStream));
    decltype(vec) vec2;
    while (blinkCount-- > 0) {
        vec2.clear();
        for (auto num : vec) {
            if (num == 0) {
                vec2.push_back(1);
            } else if (auto str = std::to_string(num); str.size() % 2 == 0) {
                vec2.push_back(std::stoll(str.substr(0, str.size() / 2)));
                vec2.push_back(std::stoll(str.substr(str.size() / 2)));
            } else {
                vec2.push_back(num * aocYear);
            }
        }
        vec.swap(vec2);
    }
    return (int)vec.size();
}

auto Day11Solution::part2(std::istream& inputStream) -> Part2ResultType {
    auto vec = ranges::to<std::vector<long long>>(views::istream<long long>(inputStream));
    std::unordered_map<long long, long long> numCount;
    for (auto num : vec) ++numCount[num];
    decltype(numCount) numCount2;
    for (int blinkCount = part2BlinkCount; blinkCount-- > 0;) {
        // numCount2.clear(); // +3ms
        for (auto& [key, val] : numCount2) val = 0;
        for (auto& [key, val] : numCount) {
            if (val == 0) continue; // -1.4ms
            if (key == 0) {
                numCount2[1] += val;
            } /*else if (auto str = std::to_string(key); str.size() % 2 == 0) {
                numCount2[std::stoll(str.substr(0, str.size() / 2))] += val;
                numCount2[std::stoll(str.substr(str.size() / 2))] += val;
            } // +7ms */
            else if (auto keyLen = (int)std::log10(key) + 1; keyLen % 2 == 0) {
                // long long divisor = (long long)std::pow(10, keyLen / 2); // +2ms
                long long divisor{1};
                for (auto halfLen = keyLen / 2; halfLen-- > 0;) divisor *= base10;
                numCount2[key / divisor] += val;
                numCount2[key % divisor] += val;
            } else {
                numCount2[key * aocYear] += val;
            }
        }
        numCount.swap(numCount2);
    }
    return ranges::fold_left(numCount, 0LL, [](long long sum, auto&& keyval) { return sum + keyval.second; });
}