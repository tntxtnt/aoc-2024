#include <day08_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <complex>
#include <cctype>
#include <ranges>
namespace views = std::views;

using CoordType = std::complex<double>;

auto Day8Solution::part1(std::istream& is) -> Part1ResultType {
    std::unordered_map<char, std::vector<CoordType>> coordMap;
    std::vector<std::string> mat;
    int rows{};
    int cols{};
    for (std::string line; is >> line; ++rows) {
        mat.push_back(line);
        for (auto [c, ch] : views::enumerate(line))
            if (std::isalnum(ch)) coordMap[ch].emplace_back((int)c, rows);
    }
    cols = (int)mat[0].size();
    auto setAntinode = [&](CoordType coord) -> bool {
        const int r = (int)coord.imag();
        const int c = (int)coord.real();
        return 0 <= r && r < rows && 0 <= c && c < cols && std::exchange(mat[r][c], '#') != '#';
    };
    Part1ResultType res{};
    for (auto&& [_, group] : coordMap)
        for (auto [i, p] : views::enumerate(group))
            for (auto [j, q] : views::enumerate(group | views::drop(i + 1)))
                res += (int)setAntinode(p + p - q) + (int)setAntinode(q + q - p);
    return res;
}

auto Day8Solution::part2(std::istream& is) -> Part2ResultType {
    std::unordered_map<char, std::vector<CoordType>> coordMap;
    std::vector<std::string> mat;
    int rows{};
    int cols{};
    for (std::string line; is >> line; ++rows) {
        mat.push_back(line);
        for (auto [c, ch] : views::enumerate(line))
            if (std::isalnum(ch)) coordMap[ch].emplace_back((int)c, rows);
    }
    cols = (int)mat[0].size();
    auto isValidNode = [&](CoordType coord) -> bool {
        const int r = (int)coord.imag();
        const int c = (int)coord.real();
        return 0 <= r && r < rows && 0 <= c && c < cols;
    };
    auto setAntinode = [&](CoordType coord) -> bool {
        const int r = (int)coord.imag();
        const int c = (int)coord.real();
        return 0 <= r && r < rows && 0 <= c && c < cols && std::exchange(mat[r][c], '#') != '#';
    };
    Part2ResultType res{};
    for (auto&& [_, group] : coordMap)
        for (auto [i, p] : views::enumerate(group))
            for (auto [j, q] : views::enumerate(group | views::drop(i + 1))) {
                const auto d = p - q;
                for (auto r = p; isValidNode(r); r += d) res += (int)setAntinode(r);
                for (auto r = q; isValidNode(r); r -= d) res += (int)setAntinode(r);
            }
    return res;
}