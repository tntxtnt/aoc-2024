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
            res.push_back(path);

    } while (std::next_permutation(begin(path), end(path)));
    return res;
}

static auto getPaths_(std::string_view input,
                      std::vector<std::string> (*getPaths)(char, char)) -> std::vector<std::string> {
    if (input.size() == 2)
        return ranges::to<std::vector>(getPaths(input[0], input[1]) |
                                       views::transform([](const auto& path) { return path + 'A'; }));
    const auto heads = getPaths(input[0], input[1]);
    const auto tails = getPaths_(input.substr(1), getPaths);
    return ranges::to<std::vector>(views::cartesian_product(heads, tails) |
                                   views::transform([](const auto& tup) -> std::string {
                                       const auto& [head, tail] = tup;
                                       return head + 'A' + tail;
                                   }));
}

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
}; // namespace numeric

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
}; // namespace directional
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
                    if (!shortestPath || path3.size() < shortestPath->size()) shortestPath = path3;
                }
            }
        }
        if (!shortestPath) return -1;
        aoc::println("      {}({})", *shortestPath, shortestPath->size());
        res += (int)shortestPath->size() * std::stoi(input);
    }
    return res;
}

auto Day21Solution::part2(std::istream& /*inputStream*/) -> Part2ResultType {
    Part2ResultType res{};
    return res;
}