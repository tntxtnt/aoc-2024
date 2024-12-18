#include <day18_solution.h>

#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

constexpr char emptyChar = '.';
constexpr char wallChar = '#';
constexpr char shortestPathChar = 'O';
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
};

auto parseInput(std::istream& inputStream, int width, int height, int wallCount) -> Map2 {
    Map2 map(std::vector<std::string>(height + 2, std::string(width + 2, emptyChar))); // with wall border around
    map.data[0] = map.data.back() = std::string(width + 2, wallChar);
    for (int row : views::iota(1) | views::take(height)) map.data[row][0] = map.data[row].back() = wallChar;
    for (int row, col; inputStream >> col && inputStream.ignore() && inputStream >> row && wallCount-- > 0;)
        map.data[row + 1][col + 1] = wallChar;
    return map;
}

auto Day18Solution::part1(std::istream& inputStream, int width, int height, int wallCount) -> Part1ResultType {
    Map2 map = parseInput(inputStream, width, height, wallCount);
    std::queue<Coord2i> queue;
    std::vector<std::vector<int>> dist(height + 2, std::vector<int>(width + 2, -1));
    queue.emplace(1, 1);
    map.data[1][1] = visitedChar;
    dist[1][1] = 0;
    while (!queue.empty()) {
        auto curr = std::move(queue.front());
        queue.pop();
        if (curr.row == height && curr.col == width) break;
        map.forEachNeighbor(curr, [&](const Coord2i& next) {
            if (map[next] != emptyChar) return;
            map[next] = visitedChar;
            dist[next.row][next.col] = dist[curr.row][curr.col] + 1;
            queue.push(next);
        });
    }
    return dist[height][width];
}

auto isExitReachable(Map2 map, int width, int height) -> bool {
    std::stack<Coord2i> stack;
    stack.emplace(1, 1);
    map.data[1][1] = visitedChar;
    while (!stack.empty()) {
        auto curr = std::move(stack.top());
        stack.pop();
        if (curr.row == height && curr.col == width) return true;
        map.forEachNeighbor(curr, [&](const Coord2i& next) {
            if (map[next] != emptyChar) return;
            map[next] = visitedChar;
            stack.push(next);
        });
    }
    return false;
}

auto Day18Solution::part2(std::istream& inputStream, int width, int height, int wallCount) -> Part2ResultType {
    Map2 map = parseInput(inputStream, width, height, wallCount);
    for (int row, col; inputStream >> col && inputStream.ignore() && inputStream >> row;) {
        map.data[row + 1][col + 1] = wallChar;
        if (!isExitReachable(map, width, height)) return fmt::format("{},{}", col, row);
    }
    return {};
}