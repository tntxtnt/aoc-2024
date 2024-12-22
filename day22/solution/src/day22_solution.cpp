#include <day22_solution.h>

#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

struct Pseudorandom {
    unsigned long long seed{};

    unsigned long long random() {
        mix(seed * 64);
        prune();
        mix(seed / 32);
        prune();
        mix(seed * 2048);
        prune();
        return (unsigned long long)seed;
    }

private:
    void prune() { seed %= 16777216; }
    void mix(unsigned long long num) { seed ^= num; }
};

auto Day22Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Part1ResultType res{};
    for (unsigned n : views::istream<unsigned>(inputStream)) {
        Pseudorandom gen{n};
        ranges::for_each(views::iota(0) | views::take(1999), [&](int /*unused*/) { gen.random(); });
        res += gen.random();
    }
    return res;
}

auto Day22Solution::part2(std::istream& inputStream) -> Part2ResultType {
    constexpr size_t changeCount = 2000;
    std::unordered_map<unsigned, int> bigMap;
    for (unsigned n : views::istream<unsigned>(inputStream)) {
        Pseudorandom gen{n};
        decltype(bigMap) map;
        for (unsigned hash{}, prev{n % 10}; unsigned i : views::iota(1) | views::take(changeCount)) {
            unsigned curr = gen.random() % 10;
            hash = ((hash << 5) | (curr - prev + 9)) & 0xFFFFF;
            if (i >= 4 && !map.contains(hash)) map[hash] = curr;
            prev = curr;
        }
        for (auto& [key, val] : map) bigMap[key] += val;
    }
    return ranges::max(bigMap | views::transform([](auto& kval) { return kval.second; }));
}