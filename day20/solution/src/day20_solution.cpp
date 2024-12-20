#include <day20_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

constexpr char emptyChar = '.';
constexpr char wallChar = '#';
constexpr char startChar = 'S';
constexpr char endChar = 'E';
constexpr char visitedChar = 'x';

struct Coord2i {
    int row;
    int col;
};

struct Map2 {
    std::vector<std::string> data;
    auto operator[](const Coord2i& pos) const -> char { return data[pos.row][pos.col]; };
    auto operator[](const Coord2i& pos) -> char& { return data[pos.row][pos.col]; };
    auto rows() const -> int { return (int)data.size(); }
    auto cols() const -> int { return data.empty() ? 0 : (int)data[0].size(); }
    auto isWallAt(const Coord2i& pos) const -> bool { return (*this)[pos] == wallChar; }
    auto isEmptyAt(const Coord2i& pos) const -> bool { return (*this)[pos] == emptyChar; }
    template <class Function>
    void forEachNeighbor(const Coord2i& curr, Function&& func) {
        std::forward<Function>(func)(Coord2i{.row = curr.row - 1, .col = curr.col});
        std::forward<Function>(func)(Coord2i{.row = curr.row + 1, .col = curr.col});
        std::forward<Function>(func)(Coord2i{.row = curr.row, .col = curr.col - 1});
        std::forward<Function>(func)(Coord2i{.row = curr.row, .col = curr.col + 1});
    }
    auto find(char chr) const -> Coord2i {
        for (int row = 0; row < rows(); ++row)
            for (int col = 0; col < cols(); ++col)
                if (data[row][col] == chr) return {row, col};
        return {-1, -1};
    }
    auto isValidCoord(const Coord2i& pos) const -> bool {
        return 0 <= pos.row && pos.row < rows() && 0 <= pos.col && pos.col < cols();
    }
};

auto getDistFromCoord(const Coord2i& coord, Map2 map) -> std::vector<std::vector<int>> {
    std::queue<Coord2i> queue;
    std::vector<std::vector<int>> dist(map.rows(), std::vector<int>(map.cols(), -1));
    queue.push(coord);
    map[coord] = visitedChar;
    dist[coord.row][coord.col] = 0;
    while (!queue.empty()) {
        auto curr = std::move(queue.front());
        queue.pop();
        map.forEachNeighbor(curr, [&](const Coord2i& next) {
            if (map.isWallAt(next) || map[next] == visitedChar) return;
            map[next] = visitedChar;
            dist[next.row][next.col] = dist[curr.row][curr.col] + 1;
            queue.push(next);
        });
    }
    return dist;
}

auto countCheatSaves(const Map2& map, int cheatTime, int leastPicosecondSaved) -> int {
    const auto endCoord = map.find(endChar);
    const auto distFromS = getDistFromCoord(map.find(startChar), map);
    const auto distFromE = getDistFromCoord(endCoord, map);
    const auto costWithoutCheat = distFromS[endCoord.row][endCoord.col];
    int res{};
    for (int row = 0; row < map.rows(); ++row) {
        for (int col = 0; col < map.cols(); ++col) {
            if (map.isWallAt({row, col})) continue;
            for (int dr = -cheatTime; dr <= cheatTime; ++dr) {
                for (int dc = -cheatTime; dc <= cheatTime; ++dc) {
                    if (const auto diff = std::abs(dr) + std::abs(dc); diff <= cheatTime) {
                        const auto jumpCoord = Coord2i{.row = row + dr, .col = col + dc};
                        if (!map.isValidCoord(jumpCoord) || map.isWallAt(jumpCoord)) continue;
                        const auto distFromStart = distFromS[row][col];
                        const auto distFromEnd = distFromE[jumpCoord.row][jumpCoord.col];
                        const auto newDist = distFromStart + diff + distFromEnd;
                        const auto saved = costWithoutCheat - newDist;
                        if (saved >= leastPicosecondSaved) ++res;
                    }
                }
            }
        }
    }
    return res;
}

auto Day20Solution::part1(std::istream& inputStream, int leastPicosecondSaved) -> Part1ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    return countCheatSaves(map, 2, leastPicosecondSaved);
}

auto Day20Solution::part2(std::istream& inputStream, int leastPicosecondSaved) -> Part2ResultType {
    Map2 map{ranges::to<std::vector<std::string>>(views::istream<LineWrapper>(inputStream))};
    return countCheatSaves(map, 20, leastPicosecondSaved);
}