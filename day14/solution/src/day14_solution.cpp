#include <day14_solution.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <array>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

struct Point2i {
    int x{};
    int y{};
};

struct BoundingBox {
    int top{};
    int left{};
    int bottom{};
    int right{};
    BoundingBox() = default;
    BoundingBox(int top, int left, int bottom, int right) : top{top}, left{left}, bottom{bottom}, right{right} {}
    auto width() const { return right - left + 1; }
    auto height() const { return bottom - top + 1; }
    auto quadrants() const -> std::array<BoundingBox, 4> {
        const auto halfW = width() / 2;
        const auto halfH = height() / 2;
        return {{{top, left, top + halfH - 1, left + halfW - 1}, //
                 {top, right - halfW + 1, top + halfH - 1, right},
                 {bottom - halfH + 1, left, bottom, left + halfW - 1},
                 {bottom - halfH + 1, right - halfW + 1, bottom, right}}};
    }
    auto contains(const Point2i& point) const -> bool {
        return left <= point.x && point.x <= right && top <= point.y && point.y <= bottom;
    }
};

static auto parseInput(std::istream& inputStream) -> std::pair<std::vector<Point2i>, std::vector<Point2i>> {
    std::vector<Point2i> positions;
    std::vector<Point2i> velocities;
    for (std::string line : views::istream<LineWrapper>(inputStream)) {
        Point2i pos;
        Point2i vel;
        std::istringstream iss{line};
        iss.ignore(2);
        iss >> pos.x;
        iss.ignore();
        iss >> pos.y;
        iss.ignore(3);
        iss >> vel.x;
        iss.ignore();
        iss >> vel.y;
        positions.emplace_back(std::move(pos));
        velocities.emplace_back(std::move(vel));
    }
    return {positions, velocities};
}

auto Day14Solution::part1(std::istream& inputStream, const int width, const int height) -> Part1ResultType {
    auto [positions, velocities] = parseInput(inputStream);
    // Loop
    constexpr int seconds = 100;
    for (int sec = 1; sec <= seconds; ++sec) {
        for (auto&& [pos, vel] : views::zip(positions, velocities)) {
            pos.x = (pos.x + vel.x + width) % width;
            pos.y = (pos.y + vel.y + height) % height;
        }
    }
    const auto bb = BoundingBox(0, 0, height - 1, width - 1);
    return ranges::fold_left(bb.quadrants(), 1, [&](auto mul, const auto& quad) {
        return mul * (int)ranges::count_if(positions, [&](const Point2i& pos) { return quad.contains(pos); });
    });
}

auto Day14Solution::part2(std::istream& inputStream, const int width, const int height) -> Part2ResultType {
    auto [positions, velocities] = parseInput(inputStream);
    // Loop
    std::vector<std::string> mat(height, std::string(width, '.'));
    for (int sec : views::iota(1)) {
        for (auto&& [pos, vel] : views::zip(positions, velocities)) {
            pos.x = (pos.x + vel.x + width) % width;
            pos.y = (pos.y + vel.y + height) % height;
        }
        for (auto& row : mat) ranges::fill(row, '.');
        for (auto& pos : positions) mat[pos.y][pos.x] = '*';
        auto foundXmasTree = [&] {
            for (int i = 0; i < std::ssize(mat) - 2; ++i)
                for (int j = 0; j < std::ssize(mat[0]); ++j)
                    if (std::string_view{mat[i + 0]}.substr(j).starts_with("..*..") &&
                        std::string_view{mat[i + 1]}.substr(j).starts_with(".***.") &&
                        std::string_view{mat[i + 2]}.substr(j).starts_with("*****"))
                        return true;
            return false;
        };
        if (foundXmasTree()) {
            for (auto& row : mat) aoc::println("{}", row);
            aoc::println("^ after {} seconds", sec);
            return sec;
        }
    }
    return -1;
}