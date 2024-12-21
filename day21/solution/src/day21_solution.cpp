#include <day21_solution.h>

#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <optional>
#include <unordered_map>
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
}; // namespace numeric
} // namespace keypad

auto Day21Solution::part1(std::istream& inputStream) -> Part1ResultType {
    return (Part1ResultType)part2(inputStream, 2);
}

auto Day21Solution::part2(std::istream& inputStream, int repeat) -> Part2ResultType {
    std::unordered_map<std::string, std::vector<Part2ResultType>> cache;
    auto dfs = [&](this auto&& dfs, const std::string& input, int level, bool isDirectional) -> Part2ResultType {
        if (cache[input].empty()) cache[input].resize(repeat + 1, -1);
        if (cache[input][level] != -1) return cache[input][level];
        Part2ResultType res{};
        for (auto [from, to] : views::adjacent<2>('A' + input)) {
            const auto paths =
                isDirectional ? keypad::directional::getPaths(from, to) : keypad::numeric::getPaths(from, to);
            if (level == 0) {
                res += ranges::min_element(paths, std::less{}, [](std::string_view sv) { return sv.size(); })->size();
            } else {
                Part2ResultType minLen{std::numeric_limits<Part2ResultType>::max()};
                for (auto& path : paths) minLen = std::min(minLen, dfs(path, level - 1, true));
                res += minLen;
            }
        }
        return cache[input][level] = res;
    };
    Part2ResultType res{};
    for (std::string input : views::istream<LineWrapper>(inputStream))
        res += dfs(input, repeat, false) * std::stoi(input);
    return res;
}