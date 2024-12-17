#include <day16_solution.h>

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <limits>
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
    int row;
    int col;
};

struct DirectedCoord2i {
    Coord2i coord;
    char dir;
    template <class Function>
    void forEachNeighbor(Function&& func) const {
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
        return data[(dCoord.coord.row * 4 * cols) + (dCoord.coord.col * 4) + directionToInt(dCoord.dir)];
    }
    auto operator[](const DirectedCoord2i& dCoord) const -> ValueType {
        return data[(dCoord.coord.row * 4 * cols) + (dCoord.coord.col * 4) + directionToInt(dCoord.dir)];
    }
};

template <class T>
concept HasDirectedCoord2i = requires(T obj) {
    { obj.dCoord } -> std::convertible_to<DirectedCoord2i>;
};

template <class T>
concept DirectedCoord2iHistory = requires(T obj) {
    { obj.hasVisited(DirectedCoord2i{}, int{}) } -> std::convertible_to<bool>;
    { obj.markVisited(DirectedCoord2i{}, int{}) };
};

template <class T, DirectedCoord2iHistory History>
    requires HasDirectedCoord2i<T> || std::is_convertible_v<T, DirectedCoord2i>
struct DirectedCoord2iMinHeap {
    struct WeightedT {
        T dCoord;
        int weight;
        auto operator>(const WeightedT& rhs) const { return weight > rhs.weight; }
    };
    std::priority_queue<WeightedT, std::vector<WeightedT>, std::greater<>> minPQ;
    History history;

    explicit DirectedCoord2iMinHeap(const Map2& map) : history(map.rows(), map.cols()) {}

    void update(T&& dCoord, int weight) {
        if (!history.hasVisited(getDirectedCoord2i(dCoord), weight)) minPQ.emplace(std::move(dCoord), weight);
    }

    template <class Function>
    auto forEach(Function&& func) -> bool {
        while (!minPQ.empty()) {
            auto [dCoord, weight] = std::move(minPQ.top());
            minPQ.pop();
            if (history.hasVisited(getDirectedCoord2i(dCoord), weight)) continue;
            history.markVisited(getDirectedCoord2i(dCoord), weight);
            if (!std::forward<Function>(func)(dCoord, weight)) return false;
        }
        return true;
    }

private:
    auto getDirectedCoord2i(const T& dCoord) -> DirectedCoord2i {
        if constexpr (std::is_convertible_v<T, DirectedCoord2i>) {
            return dCoord;
        } else {
            return dCoord.dCoord;
        }
    }
};

auto Day16Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    struct Visited {
        DirectedCoord2iMap<char> data;
        Visited(int rows, int cols) : data(rows, cols, 0) {}
        auto hasVisited(const DirectedCoord2i& dCoord, int /*unused*/) const -> bool { return data[dCoord] == 1; }
        void markVisited(const DirectedCoord2i& dCoord, int /*unused*/) { data[dCoord] = 1; }
    };
    DirectedCoord2iMinHeap<DirectedCoord2i, Visited> minHeap(map);
    // S is at bottom left corner, facing right
    minHeap.update(DirectedCoord2i{.coord = {map.rows() - 2, 1}, .dir = faceRightChar}, 0);
    // Dijkstra
    Part1ResultType endCost{INT_MAX};
    minHeap.forEach([&](const DirectedCoord2i& curr, int weight) -> bool {
        if (map.isEndAt(curr.coord)) {
            endCost = weight;
            return false;
        }
        curr.forEachNeighbor([&](DirectedCoord2i&& next, int cost) {
            if (!map.isWallAt(next.coord)) minHeap.update(std::move(next), weight + cost);
        });
        return true;
    });
    return endCost;
}

struct TrackedDirectedCoord2i {
    DirectedCoord2i dCoord;
    DirectedCoord2i from;
};

auto Day16Solution::part2(std::istream& inputStream) -> Part2ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    struct Visited {
        DirectedCoord2iMap<int> data;
        Visited(int rows, int cols) : data(rows, cols, INT_MAX) {}
        auto hasVisited(const DirectedCoord2i& dCoord, int weight) const -> bool { return data[dCoord] < weight; }
        void markVisited(const DirectedCoord2i& dCoord, int weight) { data[dCoord] = weight; }
    };
    DirectedCoord2iMinHeap<TrackedDirectedCoord2i, Visited> minHeap(map);
    // S is at bottom left corner, facing right
    minHeap.update(TrackedDirectedCoord2i{.dCoord = {.coord = {map.rows() - 2, 1}, .dir = faceRightChar},
                                          .from = {.coord = {-1, -1}, .dir = '?'}},
                   0);
    // Dijkstra with tracking
    DirectedCoord2iMap<std::vector<DirectedCoord2i>> parents(map.rows(), map.cols(), {});
    int endCost{INT_MAX};
    std::queue<DirectedCoord2i> endQ;
    minHeap.forEach([&](const TrackedDirectedCoord2i& tdCoord, int weight) -> bool {
        const auto& [curr, from] = tdCoord;
        if (weight > endCost) return false;
        parents[curr].push_back(from);
        if (map.isEndAt(curr.coord)) {
            endCost = weight;
            endQ.push(curr);
        } else {
            curr.forEachNeighbor([&](DirectedCoord2i next, int cost) {
                if (!map.isWallAt(next.coord))
                    minHeap.update(TrackedDirectedCoord2i{.dCoord = next, .from = curr}, weight + cost);
            });
        }
        return true;
    });
    // Backtrack
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
