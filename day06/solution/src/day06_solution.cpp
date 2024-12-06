#include <day06_solution.h>

#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <complex>
#include <cstdint>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;

auto Day6Solution::part1(std::istream& is) -> Part1ResultType {
    std::vector<std::string> mat;
    for (std::string line; std::getline(is, line);) mat.push_back(std::move(line));
    const int rows = (int)mat.size();
    const int cols = (int)mat[0].size();
    using CoordType = std::complex<double>;
    CoordType start;
    for (auto [r, row] : views::enumerate(mat))
        for (auto [c, ch] : views::enumerate(row))
            if (ch == '^') start.imag((int)r), start.real((int)c);
    CoordType dir{0, -1};
    for (auto curr = start;;) {
        mat[(int)curr.imag()][(int)curr.real()] = 'X';
        const auto next = curr + dir;
        const int nextR = (int)next.imag();
        const int nextC = (int)next.real();
        if (!((0 <= nextR && nextR < rows) && (0 <= nextC && nextC < cols))) break;
        if (mat[nextR][nextC] == '#') {
            dir *= CoordType(0, 1);
        } else {
            curr = next;
        }
    }
    return ranges::fold_left(mat, 0, [](int sum, std::string_view row) { return sum + (int)ranges::count(row, 'X'); });
}

auto Day6Solution::part2(std::istream& is) -> Part2ResultType {
    std::vector<std::string> mat;
    for (std::string line; std::getline(is, line);) mat.push_back(std::move(line));
    using CoordType = std::complex<double>;
    CoordType start;
    for (auto [r, row] : views::enumerate(mat))
        for (auto [c, ch] : views::enumerate(row))
            if (ch == '^') start.imag((int)r), start.real((int)c), ch = '.';
    auto hasLoop = [](const std::vector<std::string>& mat, CoordType curr) {
        const int rows = (int)mat.size();
        const int cols = (int)mat[0].size();
        std::vector<std::vector<uint8_t>> mmat(rows, std::vector<uint8_t>(cols, 0));
        CoordType dir{0, -1};
        uint8_t dirType = 1;
        for (;;) {
            auto& currVal = mmat[(int)curr.imag()][(int)curr.real()];
            if (currVal & dirType) return true;
            currVal |= dirType;
            const auto next = curr + dir;
            const int nextR = (int)next.imag();
            const int nextC = (int)next.real();
            if (!((0 <= nextR && nextR < rows) && (0 <= nextC && nextC < cols))) return false;
            if (mat[nextR][nextC] != '.') {
                dir *= CoordType(0, 1);
                if ((dirType <<= 1) > 8) dirType = 1;
            } else {
                curr = next;
            }
        }
    };
    Part2ResultType res{};
    for (auto [r, row] : views::enumerate(mat)) {
        for (auto [c, ch] : views::enumerate(row)) {
            if (ch == '.') {
                ch = 'O';
                if (hasLoop(mat, start)) ++res;
                ch = '.';
            }
        }
    }
    return res;
}