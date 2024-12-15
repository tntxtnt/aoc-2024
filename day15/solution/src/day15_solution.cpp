#include <day15_solution.h>

#include <vector>
#include <string>
#include <array>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

constexpr char botChar = '@';
constexpr char emptyChar = '.';
constexpr char wallChar = '#';
constexpr char box1Char = 'O';
constexpr char box2LChar = '[';
constexpr char box2RChar = ']';

struct Coord2i {
    int row{};
    int col{};
    Coord2i() = default;
    Coord2i(int row, int col) : row{row}, col{col} {}
    auto operator+=(const Coord2i& rhs) -> Coord2i& {
        row += rhs.row;
        col += rhs.col;
        return *this;
    }
    auto operator+(const Coord2i& rhs) const -> Coord2i { return {row + rhs.row, col + rhs.col}; }
    auto operator==(const Coord2i& rhs) const -> bool { return row == rhs.row && col == rhs.col; }
    auto operator!=(const Coord2i& rhs) const -> bool { return !(*this == rhs); }
    auto rightNext() const -> Coord2i { return {row, col + 1}; }
};

struct Map2 {
    std::vector<std::string> data;
    auto operator[](const Coord2i& pos) const -> char { return data[pos.row][pos.col]; };
    auto operator[](const Coord2i& pos) -> char& { return data[pos.row][pos.col]; };
    auto rows() const -> int { return (int)data.size(); }
    auto cols() const -> int { return data.empty() ? 0 : (int)data[0].size(); }
    void swapCells(const Coord2i& pos1, const Coord2i& pos2) {
        std::swap(data[pos1.row][pos1.col], data[pos2.row][pos2.col]);
    }
    auto isWallAt(const Coord2i& pos) const -> bool { return (*this)[pos] == wallChar; }
    auto isEmptyAt(const Coord2i& pos) const -> bool { return (*this)[pos] == emptyChar; }
    auto isBox1At(const Coord2i& pos) const -> bool { return (*this)[pos] == box1Char; }
    auto isBox2LAt(const Coord2i& pos) const -> bool { return (*this)[pos] == box2LChar; }
    auto isBox2RAt(const Coord2i& pos) const -> bool { return (*this)[pos] == box2RChar; }
    auto isBox2At(const Coord2i& pos) const -> bool { return isBox2LAt(pos) || isBox2RAt(pos); }
    auto getBox2Pos(const Coord2i& pos) const -> Coord2i { return {pos.row, pos.col - (int)isBox2RAt(pos)}; }
    auto get2(const Coord2i& pos) const -> std::string_view {
        return std::string_view{data[pos.row]}.substr(pos.col, 2);
    }
    template <class Function>
    void forEachCell(Function&& func) {
        for (auto [row, dataRow] : views::enumerate(data))
            for (auto [col, val] : views::enumerate(dataRow)) std::forward<Function>(func)((int)row, (int)col, val);
    }
};

static auto getDir(char dir) -> Coord2i {
    if (dir == '^') return {-1, 0};
    if (dir == '<') return {0, -1};
    if (dir == 'v') return {1, 0};
    if (dir == '>') return {0, 1};
    return {};
}

auto Day15Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Map2 map;
    Coord2i botPos{-1, -1};
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        if (line.empty()) break;
        if (const auto col = line.find(botChar); col != std::string::npos) botPos = Coord2i{map.rows(), (int)col};
        map.data.emplace_back(std::move(line));
    }
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        for (char direction : line) {
            const Coord2i dir = getDir(direction);
            const Coord2i nextPos = botPos + dir;
            if (map.isWallAt(nextPos)) continue;
            if (map.isBox1At(nextPos)) {
                Coord2i nextNonBoxPos = nextPos;
                while (map.isBox1At(nextNonBoxPos)) nextNonBoxPos += dir; // Find space behind
                if (map.isWallAt(nextNonBoxPos)) continue;                // No space behind
                map.swapCells(nextPos, nextNonBoxPos);                    // Has space behind, move boxes
            }
            map.swapCells(nextPos, botPos);
            botPos = nextPos;
        }
    }
    Part1ResultType res{};
    map.forEachCell([&](int row, int col, char val) {
        if (val == box1Char) res += row * 100 + col;
    });
    return res;
}

auto Day15Solution::part2(std::istream& inputStream) -> Part2ResultType {
    Map2 map;
    Coord2i botPos{-1, -1};
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        if (line.empty()) break;
        std::string newLine;
        newLine.reserve(line.size() * 2);
        for (char chr : line) {
            if (chr == wallChar || chr == emptyChar) {
                newLine += chr;
                newLine += chr;
            } else if (chr == botChar) {
                newLine += botChar;
                newLine += emptyChar;
            } else if (chr == box1Char) {
                newLine += box2LChar;
                newLine += box2RChar;
            }
        }
        if (const auto col = newLine.find(botChar); col != std::string::npos) botPos = Coord2i{map.rows(), (int)col};
        map.data.emplace_back(std::move(newLine));
    }
    auto moveBoxesVertical = [&](Coord2i boxPos, const Coord2i& dir) -> bool {
        auto canMoveBoxes = [&](this auto&& canMoveBoxes, Coord2i boxPos) -> bool {
            boxPos = map.getBox2Pos(boxPos);
            const auto nextPos = boxPos + dir;
            auto nextCells = map.get2(nextPos);
            // Has wall in new position
            if (nextCells.contains(wallChar)) return false;
            // Empty spaces in new position
            if (nextCells == "..") return true;
            // Has box(es) in new position: .[ [] ]. ][
            if (nextCells == "[]" || nextCells == "].") return canMoveBoxes(nextPos);
            if (nextCells == ".[") return canMoveBoxes(nextPos.rightNext());
            return canMoveBoxes(nextPos) && canMoveBoxes(nextPos.rightNext());
        };
        auto moveBoxes = [&](this auto&& moveBoxes, Coord2i boxPos) -> void {
            boxPos = map.getBox2Pos(boxPos);
            const Coord2i posLeft = boxPos + dir;
            const Coord2i posRight = posLeft.rightNext();
            if (!map.isEmptyAt(posLeft)) moveBoxes(posLeft);
            if (map.isBox2LAt(posRight)) moveBoxes(posRight);
            map.swapCells(posLeft, boxPos);
            map.swapCells(posRight, boxPos.rightNext());
        };
        if (!canMoveBoxes(boxPos)) return false;
        moveBoxes(boxPos);
        return true;
    };
    auto moveBoxesHorizontal = [&](const Coord2i& boxPos, const Coord2i& dir) -> bool {
        // Find behind position
        Coord2i behindBoxesPos = boxPos;
        while (map.isBox2At(behindBoxesPos)) behindBoxesPos += dir;
        if (map.isWallAt(behindBoxesPos)) return false;
        // Behind is empty space
        for (Coord2i coord = boxPos; coord != behindBoxesPos; coord += dir)
            map[coord] = map[coord] == box2LChar ? box2RChar : box2LChar;
        map.swapCells(behindBoxesPos, boxPos);
        return true;
    };
    auto moveBoxes = [&](const Coord2i& boxPos, const Coord2i& dir) -> bool {
        return dir.row == 0 ? moveBoxesHorizontal(boxPos, dir) : moveBoxesVertical(boxPos, dir);
    };
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        for (char direction : line) {
            const Coord2i dir = getDir(direction);
            const Coord2i nextPos = botPos + dir;
            if (map.isWallAt(nextPos)) continue;
            if (map.isEmptyAt(nextPos) || moveBoxes(nextPos, dir)) {
                map.swapCells(nextPos, botPos);
                botPos = nextPos;
            }
        }
    }
    Part1ResultType res{};
    map.forEachCell([&](int row, int col, char val) {
        if (val == box2LChar) res += row * 100 + col;
    });
    return res;
}