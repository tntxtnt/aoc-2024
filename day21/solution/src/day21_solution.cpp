#include <day21_solution.h>

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <optional>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

struct Coord2i {
    int row;
    int col;
    auto operator-(const Coord2i& rhs) const -> Coord2i { return {row - rhs.row, col - rhs.col}; }
    void move_(char dir) { dir == '^' ? --row : dir == 'v' ? ++row : dir == '<' ? --col : ++col; }
    auto operator!=(const Coord2i& rhs) const -> bool { return row != rhs.row || col != rhs.col; }
};

namespace keypad
{
static auto getPaths_(const Coord2i& fromPos, const Coord2i& toPos,
                      const Coord2i& invalidPos) -> std::vector<std::string> {
    const auto diff = toPos - fromPos;
    std::string path;
    path.insert(path.size(), std::abs(diff.row), diff.row < 0 ? '^' : 'v');
    path.insert(path.size(), std::abs(diff.col), diff.col < 0 ? '<' : '>');
    ranges::sort(path);
    std::vector<std::string> res;
    do {
        if (ranges::all_of(path, [&, pos = fromPos](char dir) mutable -> bool {
                pos.move_(dir);
                return pos != invalidPos;
            }))
            res.push_back(path + 'A');

    } while (std::next_permutation(begin(path), end(path)));
    return res;
}

static auto getPaths_(std::string_view input,
                      std::vector<std::string> (*getPaths)(char, char)) -> std::vector<std::string> {
    if (input.size() == 2) return getPaths(input[0], input[1]);
    const auto heads = getPaths(input[0], input[1]);
    const auto tails = getPaths_(input.substr(1), getPaths);
    return ranges::to<std::vector>(views::cartesian_product(heads, tails) |
                                   views::transform([](const auto& tup) -> std::string {
                                       const auto& [head, tail] = tup;
                                       return head + tail;
                                   }));
}

namespace directional
{
    constexpr static inline Coord2i invalidPos{0, 0};
    constexpr static inline Coord2i keyA{0, 2};
    constexpr static inline Coord2i keyUp{0, 1};
    constexpr static inline Coord2i keyLeft{1, 0};
    constexpr static inline Coord2i keyDown{1, 1};
    constexpr static inline Coord2i keyRight{1, 2};
    constexpr static auto getPos(char key) -> Coord2i {
        return key == 'A'   ? keyA
               : key == '^' ? keyUp
               : key == '<' ? keyLeft
               : key == 'v' ? keyDown
               : key == '>' ? keyRight
                            : invalidPos;
    }
    static auto getPaths(char from, char to) -> std::vector<std::string> {
        return getPaths_(getPos(from), getPos(to), invalidPos);
    }
    static auto getPaths(std::string_view input) -> std::vector<std::string> {
        return getPaths_(input, getPaths);
    }
    static auto getNextMinPath(char from, char to) -> std::string {
        std::vector<std::string> candidates;
        std::string res;
        size_t minLen{std::numeric_limits<size_t>::max()};
        for (const auto& path : getPaths(from, to)) {
            size_t shortestLen{std::numeric_limits<size_t>::max()};
            for (const auto& path2 : getPaths('A' + path))
                for (const auto& path3 : getPaths('A' + path2)) shortestLen = std::min(shortestLen, path3.size());
            if (shortestLen <= minLen) { // must be <=
                minLen = shortestLen;
                res = path;
            }
        }
        return res;
    }
}; // namespace directional

namespace numeric
{
    constexpr static inline std::array<Coord2i, 11> keyPos{
        {{3, 1}, {2, 0}, {2, 1}, {2, 2}, {1, 0}, {1, 1}, {1, 2}, {0, 0}, {0, 1}, {0, 2}, {3, 2}}};
    constexpr static inline Coord2i invalidPos{3, 0};
    constexpr static auto getPos(char key) -> Coord2i {
        return keyPos[key == 'A' ? 10 : key - '0'];
    }
    static auto getPaths(char from, char to) -> std::vector<std::string> {
        return getPaths_(getPos(from), getPos(to), invalidPos);
    }
    static auto getPaths(std::string_view input) -> std::vector<std::string> {
        return getPaths_(input, getPaths);
    }
    static auto getNextMinPath(char from, char to) -> std::string {
        std::string res;
        size_t minLen{std::numeric_limits<size_t>::max()};
        for (const auto& path : getPaths(from, to)) {
            size_t shortestLen{std::numeric_limits<size_t>::max()};
            for (const auto& path2 : directional::getPaths('A' + path))
                for (const auto& path3 : directional::getPaths('A' + path2))
                    shortestLen = std::min(shortestLen, path3.size());
            if (shortestLen <= minLen) { // must be <=
                minLen = shortestLen;
                res = path;
            }
        }
        return res;
    }
}; // namespace numeric
} // namespace keypad

auto Day21Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string input : views::istream<LineWrapper>(inputStream)) {
        std::optional<std::string> shortestPath;
        aoc::println("{}({})", input, input.size());
        for (const auto& path1 : keypad::numeric::getPaths('A' + input)) {
            // aoc::println("  {}({})", path1, path1.size());
            for (const auto& path2 : keypad::directional::getPaths('A' + path1)) {
                // aoc::println("    {}({})", path2, path2.size());
                for (const auto& path3 : keypad::directional::getPaths('A' + path2)) {
                    // aoc::println("      {}({})", path3, path3.size());
                    if (!shortestPath || path3.size() < shortestPath->size()) {
                        if (shortestPath) aoc::println("----- {}({})", *shortestPath, shortestPath->size());
                        shortestPath = path3;
                    }
                }
            }
        }
        if (!shortestPath) return -1;
        aoc::println("      {}({})", *shortestPath, shortestPath->size());
        res += (int)shortestPath->size() * std::stoi(input);
    }
    return res;
}

struct Path {
    long long data[128][128]{};
    template <class Func>
    constexpr void forEachNonEmptyCell(Func&& func) {
        for (int from = 0; from < 128; ++from)
            for (int to = 0; to < 128; ++to)
                if (data[from][to] > 0)
                    if constexpr (requires(Func f) { f(0LL); }) {
                        std::forward<Func>(func)(data[from][to]);
                    } else if constexpr (requires(Func f) { f(0, 0, 0LL); }) {
                        std::forward<Func>(func)(from, to, data[from][to]);
                    }
    }
};

auto Day21Solution::part2(std::istream& inputStream) -> Part2ResultType {
    using namespace std::literals;
    Part2ResultType res{};
    std::string map[128][128]{};
    for (char from : "^v<>A"sv)
        for (char to : "^v<>A"sv) map[from][to] = keypad::directional::getNextMinPath(from, to);
    for (char from : "0123456789A"sv)
        for (char to : "0123456789A"sv) map[from][to] = keypad::numeric::getNextMinPath(from, to);
    for (std::string input : views::istream<LineWrapper>(inputStream)) {
        Path path;
        auto genNext = [](std::string_view path, long long cnt, Path& out) {
            out.data['A'][path[0]] += cnt;
            for (auto [from, to] : views::adjacent<2>(path)) out.data[from][to] += cnt;
        };
        genNext(input, 1, path);
        for (int repeat = 26; repeat-- > 0;) {
            Path nextPath;
            path.forEachNonEmptyCell([&](int from, int to, long long cnt) { genNext(map[from][to], cnt, nextPath); });
            path = nextPath;
        }
        long long len{};
        path.forEachNonEmptyCell([&](long long cnt) { len += cnt; });
        res += len * std::stoi(input);
    }
    return res;
}