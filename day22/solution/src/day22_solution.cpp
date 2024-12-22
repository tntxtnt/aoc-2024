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
    using value_type = unsigned;
    value_type seed{};
    auto random() -> value_type {
        seed = (seed ^ (seed * 64ULL)) % 16777216;
        seed = (seed ^ (seed / 32ULL)) % 16777216;
        seed = (seed ^ (seed * 2048ULL)) % 16777216;
        return seed;
    }
};

constexpr unsigned changeCount = 2000;

auto Day22Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Part1ResultType res{};
    for (unsigned n : views::istream<unsigned>(inputStream)) {
        Pseudorandom gen{n};
        ranges::for_each(views::iota(0) | views::take(changeCount - 1), [&](int /*unused*/) { gen.random(); });
        res += gen.random();
    }
    return res;
}

consteval auto powi(unsigned n, unsigned exp) -> unsigned {
    unsigned res{1};
    for (; exp > 0; exp >>= 1, n *= n)
        if (exp % 2 == 1) res *= n;
    return res;
}

auto Day22Solution::part2(std::istream& inputStream) -> Part2ResultType {
    constexpr unsigned patternSize = 4;
    constexpr unsigned base = 10;
    constexpr unsigned deltaMagnitude = base - 1;
    constexpr unsigned deltaSize = 2 * deltaMagnitude + 1;
    constexpr unsigned hashSize = powi(deltaSize, patternSize);
    std::vector<int> bigMap(hashSize);
    for (Pseudorandom::value_type n : views::istream<unsigned>(inputStream)) {
        Pseudorandom gen{n};
        decltype(bigMap) map(hashSize, -1);
        for (unsigned hash{}, prev{n % base}; unsigned i : views::iota(1) | views::take(changeCount)) {
            unsigned curr = gen.random() % base;
            hash = (hash * deltaSize + curr - prev + deltaMagnitude) % hashSize;
            if (i >= patternSize && map[hash] == -1) map[hash] = (int)curr;
            prev = curr;
        }
        for (auto [key, val] : views::enumerate(map))
            if (val != -1) bigMap[key] += val;
    }
    return ranges::max(bigMap);
}