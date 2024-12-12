#include <day12_solution.h>

#include <vector>
#include <string>
#include <cctype>
#include <array>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>
#include <fmt/ranges.h>
#include <limits>

const std::array<std::pair<int, int>, 4> neightborDiffs{{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};
const std::array<std::pair<int, int>, 8> neightborDiffs2{
    {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};
constexpr std::string spaceChar = " ";
constexpr std::string borderChar = ".";
constexpr std::string horiBorderChar = "─";
constexpr std::string vertBorderChar = "│";
constexpr std::string topLeftCornerChar = "┌";
constexpr std::string topRightCornerChar = "┐";
constexpr std::string bottomLeftCornerChar = "└";
constexpr std::string bottomRightCornerChar = "┘";
constexpr std::string doubleCornerChar = "┼";

static auto isBorderChar(std::string_view ch) {
    return ch == borderChar || ch == horiBorderChar || ch == vertBorderChar || ch == topLeftCornerChar ||
           ch == topRightCornerChar || ch == bottomLeftCornerChar || ch == bottomRightCornerChar ||
           ch == doubleCornerChar;
}

auto Day12Solution::part1(std::istream& inputStream) -> Part1ResultType {
    auto mat = ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream));
    auto isValidPos = [&](int r, int c) { return 0 <= r && r < (int)mat.size() && 0 <= c && c < (int)mat[0].size(); };
    auto dfs = [&](int r, int c) -> int {
        const char val = mat[r][c];
        const char valLower = (char)std::tolower(val);
        int perimeter{};
        int area{};
        auto collect = [&](this auto&& collect, int i, int j) -> void {
            ++area;
            mat[i][j] = valLower;
            for (auto [dr, dc] : neightborDiffs) {
                if (const int u = i + dr, v = j + dc; isValidPos(u, v)) {
                    if (mat[u][v] == val) {
                        collect(u, v);
                    } else if (mat[u][v] != valLower) {
                        ++perimeter;
                    }
                } else {
                    ++perimeter;
                }
            }
        };
        collect(r, c);
        // aoc::println("{}: {} {}", val, area, perimeter);
        return perimeter * area;
    };
    Part1ResultType res{};
    for (auto [r, row] : views::enumerate(mat))
        for (auto [c, val] : views::enumerate(row))
            if (std::isupper(val)) res += dfs((int)r, (int)c);
    return res;
}

auto Day12Solution::part2(std::istream& inputStream) -> Part2ResultType {
    auto mat = ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream));
    auto isValidPos = [&](int r, int c) { return 0 <= r && r < (int)mat.size() && 0 <= c && c < (int)mat[0].size(); };
    struct BoungindBox {
        int top{};
        int bottom{};
        int left{};
        int right{};
        BoungindBox() = default;
        BoungindBox(const std::vector<std::pair<int, int>>& coords) : top{INT_MAX}, left{INT_MAX} {
            for (auto [r, c] : coords) {
                top = std::min(top, r);
                bottom = std::max(bottom, r);
                left = std::min(left, c);
                right = std::max(right, c);
            }
        }
        int width() const { return right - left + 1; }
        int height() const { return bottom - top + 1; }
    };
    auto dfs = [&](int sr, int sc) -> int {
        const char val = mat[sr][sc];
        const char valLower = (char)std::tolower(val);
        std::vector<std::pair<int, int>> coords;
        auto collect = [&](this auto&& collect, int r, int c) -> void {
            coords.emplace_back(r, c);
            mat[r][c] = valLower;
            for (auto [dr, dc] : neightborDiffs)
                if (const int nr = r + dr, nc = c + dc; isValidPos(nr, nc) && mat[nr][nc] == val) collect(nr, nc);
        };
        collect(sr, sc);
        const auto bb = BoungindBox(coords);
        // aoc::println("{}: {}", val, coords);
        // aoc::println("bb = top {}, bottom {}, left {}, right {}", bb.top, bb.bottom, bb.left, bb.right);
        // Build new map
        std::vector<std::vector<std::string>> mat2(bb.height() * 2 + 1,
                                                   std::vector<std::string>(bb.width() * 2 + 1, spaceChar));
        const std::string valStr(1, val);
        const std::string valLowerStr(1, valLower);
        for (auto [r, c] : coords) mat[r][c] = val;
        for (auto [r, c] : coords) {
            const int r2 = (r - bb.top) * 2 + 1;
            const int c2 = (c - bb.left) * 2 + 1;
            mat2[r2][c2] = valStr;
            for (auto [dr, dc] : neightborDiffs)
                if (const int nr = r + dr, nc = c + dc; isValidPos(nr, nc) && mat[nr][nc] == val)
                    mat2[r2 + dr][c2 + dc] = valLowerStr;
        }
        for (auto [r, c] : coords) mat[r][c] = valLower;
        // aoc::println("mat2 =");
        /*for (auto& line : mat2) {
            for (auto& str : line) aoc::print("{}", std::islower(str[0]) ? " " : str);
            aoc::println("");
        }*/
        // Mark border
        auto isValidPos2 = [&](int r, int c) {
            return 0 <= r && r < (int)mat2.size() && 0 <= c && c < (int)mat2[0].size();
        };
        for (auto [r, row] : views::enumerate(mat2)) {
            for (auto [c, val2] : views::enumerate(row)) {
                if (val2 != spaceChar) continue;
                if (ranges::any_of(neightborDiffs2, [&](const auto& diff) {
                        auto [dr, dc] = diff;
                        const int nr = (int)r + dr;
                        const int nc = (int)c + dc;
                        return isValidPos2(nr, nc) && std::toupper(mat2[nr][nc][0]) == val;
                    }))
                    val2 = borderChar;
            }
        }
        // aoc::println("mat2 with border =");
        /*for (auto& line : mat2) {
            for (auto& str : line) aoc::print("{}", std::islower(str[0]) ? " " : str);
            aoc::println("");
        }*/
        // Count corners
        int numberOfSides{}; // is also number of corners
        for (auto [r, row] : views::enumerate(mat2)) {
            for (auto [c, val2] : views::enumerate(row)) {
                if (val2 != borderChar) continue;
                BoungindBox neighborCnt;
                for (auto [dr, dc] : neightborDiffs) {
                    const int nr = (int)r + dr;
                    const int nc = (int)c + dc;
                    if (isValidPos2(nr, nc) && isBorderChar(mat2[nr][nc])) {
                        if (dc == 1)
                            neighborCnt.right = 1;
                        else if (dc == -1)
                            neighborCnt.left = 1;
                        if (dr == 1)
                            neighborCnt.bottom = 1;
                        else if (dr == -1)
                            neighborCnt.top = 1;
                    }
                }
                int horizontalNeighborCnt{neighborCnt.left + neighborCnt.right};
                int verticalNeighborCnt{neighborCnt.top + neighborCnt.bottom};
                if (horizontalNeighborCnt > 0 && verticalNeighborCnt > 0) {
                    numberOfSides += verticalNeighborCnt;
                    if (verticalNeighborCnt == 2) {
                        val2 = doubleCornerChar;
                    } else if (neighborCnt.top == 1) {
                        val2 = neighborCnt.left == 1 ? bottomRightCornerChar : bottomLeftCornerChar;
                    } else {
                        val2 = neighborCnt.left == 1 ? topRightCornerChar : topLeftCornerChar;
                    }
                } else if (horizontalNeighborCnt > 0) {
                    val2 = horiBorderChar;
                } else if (verticalNeighborCnt > 0) {
                    val2 = vertBorderChar;
                } else {
                    val2 = spaceChar;
                }
            }
        }
        // aoc::println("mat2 with border and corners =");
        /*for (auto& line : mat2) {
            for (auto& str : line) aoc::print("{}", std::islower(str[0]) ? " " : str);
            aoc::println("");
        }*/
        // aoc::println("sides = {}", numberOfSides);
        // aoc::println("");
        return (int)coords.size() * numberOfSides;
    };
    Part1ResultType res{};
    for (auto [r, row] : views::enumerate(mat))
        for (auto [c, val] : views::enumerate(row))
            if (std::isupper(val)) res += dfs((int)r, (int)c);
    return res;
}