#include <day16_solution.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
using namespace std::literals;
#include <aoc/utils.h>

constexpr char emptyChar = '.';
constexpr char wallChar = '#';
constexpr char startChar = 'S';
constexpr char endChar = 'E';
constexpr char faceUpChar = '^';
constexpr char faceDownChar = 'v';
constexpr char faceLeftChar = '<';
constexpr char faceRightChar = '>';
constexpr char onMinPathChar = 'O';
constexpr int goStraigtCost = 1;
constexpr int makeTurnCost = 1001;

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
};

struct DirectedCoord2i {
    Coord2i coord;
    char dir{};
    auto operator==(const DirectedCoord2i& rhs) const -> bool { return dir == rhs.dir && coord == rhs.coord; }
    template <class Function>
    void forEachNeighbor(Function&& func) {
        if (dir == faceUpChar) {
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row - 1, coord.col}, .dir = faceUpChar},
                                         goStraigtCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col - 1}, .dir = faceLeftChar},
                                         makeTurnCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col + 1}, .dir = faceRightChar},
                                         makeTurnCost);
        } else if (dir == faceDownChar) {
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row + 1, coord.col}, .dir = faceDownChar},
                                         goStraigtCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col - 1}, .dir = faceLeftChar},
                                         makeTurnCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col + 1}, .dir = faceRightChar},
                                         makeTurnCost);
        } else if (dir == faceLeftChar) {
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col - 1}, .dir = faceLeftChar},
                                         goStraigtCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row - 1, coord.col}, .dir = faceUpChar},
                                         makeTurnCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row + 1, coord.col}, .dir = faceDownChar},
                                         makeTurnCost);
        } else if (dir == faceRightChar) {
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row, coord.col + 1}, .dir = faceRightChar},
                                         goStraigtCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row - 1, coord.col}, .dir = faceUpChar},
                                         makeTurnCost);
            std::forward<Function>(func)(DirectedCoord2i{.coord = {coord.row + 1, coord.col}, .dir = faceDownChar},
                                         makeTurnCost);
        }
    }
};

struct WeightedDirectedCoord2i {
    DirectedCoord2i dCoord;
    int weight{};
    auto operator<(const WeightedDirectedCoord2i& rhs) const -> bool { return weight > rhs.weight; }
};

struct TracedWeightedDirectedCoord2i {
    WeightedDirectedCoord2i wdCoord;
    DirectedCoord2i from{};
    auto operator<(const TracedWeightedDirectedCoord2i& rhs) const -> bool { return wdCoord < rhs.wdCoord; }
};

struct Map2 {
    std::vector<std::string> data;
    auto operator[](const Coord2i& pos) const -> char { return data[pos.row][pos.col]; };
    auto operator[](const Coord2i& pos) -> char& { return data[pos.row][pos.col]; };
    auto rows() const -> int { return (int)data.size(); }
    auto cols() const -> int { return data.empty() ? 0 : (int)data[0].size(); }
    auto isWallAt(const Coord2i& pos) const -> bool { return (*this)[pos] == wallChar; }
    auto isEmptyAt(const Coord2i& pos) const -> bool { return (*this)[pos] == emptyChar; }
    auto isStartAt(const Coord2i& pos) const -> bool { return (*this)[pos] == startChar; }
    auto isEndAt(const Coord2i& pos) const -> bool { return (*this)[pos] == endChar; }
    auto isVisitedAt(const Coord2i& pos) const -> bool { return "^<v>"sv.find((*this)[pos]) != std::string::npos; }
};

auto directionToInt(char dir) -> int {
    if (dir == '^') return 0;
    if (dir == '<') return 1;
    if (dir == 'v') return 2;
    if (dir == '>') return 3;
    return -1;
}

template <class ValueType>
struct DirectedCoord2iMap {
    int rows;
    int cols;
    std::vector<ValueType> data;
    DirectedCoord2iMap(int rows, int cols, const ValueType& initValue)
    : rows{rows}, cols{cols}, data(rows * cols * 4, initValue) {}
    auto operator[](const DirectedCoord2i& dCoord) -> ValueType& {
        const auto& [coord, dir] = dCoord;
        const auto& [row, col] = coord;
        return data[(row * 4 * cols) + (col * 4) + directionToInt(dir)];
    }
};

auto Day16Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    // S is at bottom left corner, facing right
    std::priority_queue<WeightedDirectedCoord2i> minPQ;
    minPQ.emplace(DirectedCoord2i{.coord = {map.rows() - 2, 1}, .dir = faceRightChar}, 0);
    DirectedCoord2iMap<char> visited(map.rows(), map.cols(), 0);
    while (!minPQ.empty()) {
        auto [curr, weight] = minPQ.top();
        minPQ.pop();
        if (visited[curr] == 1) continue;
        visited[curr] = 1;
        if (map.isEndAt(curr.coord)) return weight;
        curr.forEachNeighbor([&](const DirectedCoord2i& next, int cost) {
            if (!map.isWallAt(next.coord) && visited[next] != 1) minPQ.emplace(next, weight + cost);
        });
    }
    return -1;
}

auto Day16Solution::part2(std::istream& inputStream) -> Part2ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    // S is at bottom left corner, facing right
    std::priority_queue<TracedWeightedDirectedCoord2i> minPQ;
    minPQ.emplace(WeightedDirectedCoord2i{.dCoord = DirectedCoord2i{.coord = {map.rows() - 2, 1}, .dir = faceRightChar},
                                          .weight = 0},
                  DirectedCoord2i{.coord = {-1, -1}, .dir = '?'});
    DirectedCoord2iMap<int> minCost(map.rows(), map.cols(), INT_MAX);
    DirectedCoord2iMap<std::vector<DirectedCoord2i>> parents(map.rows(), map.cols(), {});
    int endCost{INT_MAX};
    std::queue<DirectedCoord2i> endQ;
    while (!minPQ.empty()) {
        auto [wcurr, from] = minPQ.top();
        auto [curr, weight] = wcurr;
        minPQ.pop();
        if (minCost[curr] < weight) continue;
        minCost[curr] = weight;
        if (weight > endCost) break;
        parents[curr].push_back(from);
        if (map.isEndAt(curr.coord)) {
            endCost = weight;
            endQ.push(curr);
        } else {
            curr.forEachNeighbor([&](DirectedCoord2i next, int cost) {
                if (!map.isWallAt(next.coord) && minCost[next] >= weight + cost)
                    minPQ.emplace(WeightedDirectedCoord2i{.dCoord = next, .weight = weight + cost}, curr);
            });
        }
    }
    DirectedCoord2iMap<char> traced(map.rows(), map.cols(), 0);
    while (!endQ.empty()) {
        auto curr = endQ.front();
        endQ.pop();
        if (curr.coord.row == -1) continue;
        if (traced[curr] == 1) continue;
        traced[curr] = 1;
        map[curr.coord] = onMinPathChar;
        for (auto& next : parents[curr]) endQ.push(next);
    }
    return (int)ranges::fold_left(map.data, 0,
                                  [](auto sum, auto& line) { return sum + (int)ranges::count(line, onMinPathChar); });
}