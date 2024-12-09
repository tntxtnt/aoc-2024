#include <day04_solution.h>
#include <vector>
#include <string>
#include <ranges>
namespace views = std::views;
#include <aoc/utils.h>

auto Day4Solution::part1(std::istream& is) -> Part1ResultType {
    std::vector<std::string> mat(3);
    for (std::string line : views::istream<LineWrapper>(is)) mat.emplace_back(std::format("...{}...", line));
    for (size_t i = 0; i < 3; ++i) {
        mat[i].resize(mat[3].size(), '.');
        mat.push_back(mat[i]);
    }
    Part1ResultType res{};
    for (size_t r = 3; r < mat.size() - 3; ++r) {
        for (size_t c = 3; c < mat[r].size() - 3; ++c) {
            if (mat[r][c] != 'X') continue;
            res += (int)(mat[r][c + 1] == 'M' && mat[r][c + 2] == 'A' && mat[r][c + 3] == 'S') +
                   (int)(mat[r][c - 1] == 'M' && mat[r][c - 2] == 'A' && mat[r][c - 3] == 'S') +
                   (int)(mat[r + 1][c] == 'M' && mat[r + 2][c] == 'A' && mat[r + 3][c] == 'S') +
                   (int)(mat[r - 1][c] == 'M' && mat[r - 2][c] == 'A' && mat[r - 3][c] == 'S') +
                   (int)(mat[r + 1][c + 1] == 'M' && mat[r + 2][c + 2] == 'A' && mat[r + 3][c + 3] == 'S') +
                   (int)(mat[r - 1][c + 1] == 'M' && mat[r - 2][c + 2] == 'A' && mat[r - 3][c + 3] == 'S') +
                   (int)(mat[r + 1][c - 1] == 'M' && mat[r + 2][c - 2] == 'A' && mat[r + 3][c - 3] == 'S') +
                   (int)(mat[r - 1][c - 1] == 'M' && mat[r - 2][c - 2] == 'A' && mat[r - 3][c - 3] == 'S');
        }
    }
    return res;
}

auto Day4Solution::part2(std::istream& is) -> Part2ResultType {
    std::vector<std::string> mat(1);
    for (std::string line : views::istream<LineWrapper>(is)) mat.emplace_back(std::format(".{}.", line));
    mat[0].resize(mat[1].size(), '.');
    mat.push_back(mat[0]);
    Part2ResultType res{};
    for (size_t r = 1; r < mat.size() - 1; ++r) {
        for (size_t c = 1; c < mat[r].size() - 1; ++c) {
            if (mat[r][c] != 'A') continue;
            res += (int)(((mat[r - 1][c - 1] == 'M' && mat[r + 1][c + 1] == 'S') ||
                          (mat[r - 1][c - 1] == 'S' && mat[r + 1][c + 1] == 'M')) &&
                         ((mat[r - 1][c + 1] == 'M' && mat[r + 1][c - 1] == 'S') ||
                          (mat[r - 1][c + 1] == 'S' && mat[r + 1][c - 1] == 'M')));
        }
    }
    return res;
}