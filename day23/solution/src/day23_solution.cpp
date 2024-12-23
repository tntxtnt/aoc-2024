#include <day23_solution.h>

#include <vector>
#include <string>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

auto Day23Solution::part1(std::istream& inputStream) -> Part1ResultType {
    std::unordered_map<std::string, std::unordered_set<std::string>> adj;
    std::unordered_set<std::string> vertices;
    std::vector<std::pair<std::string, std::string>> clique2s;
    for (std::string lhs, rhs; std::getline(inputStream, lhs, '-') && std::getline(inputStream, rhs);) {
        adj[lhs].insert(rhs);
        adj[rhs].insert(lhs);
        vertices.insert(lhs);
        vertices.insert(rhs);
        clique2s.emplace_back(lhs, rhs);
    }
    Part1ResultType res{};
    for (const auto& [vt1, vt2] : clique2s)
        for (const auto& vt3 : vertices) {
            if (vt3 == vt1 || vt3 == vt2) continue;
            if (adj[vt3].contains(vt1) && adj[vt3].contains(vt2))
                res += vt1[0] == 't' || vt2[0] == 't' || vt3[0] == 't';
        }
    return res / 3;
}

auto Day23Solution::part2(std::istream& inputStream) -> Part2ResultType {
    auto toInt = [](std::string_view str) -> int { return (str[0] - 'a') * 26 + str[1] - 'a'; };
    std::vector<std::vector<bool>> adjMat(26 * 26, std::vector<bool>(26 * 26, false));
    std::vector<std::string> vertices;
    std::unordered_set<std::string> cliques;
    for (std::string lhs, rhs; std::getline(inputStream, lhs, '-') && std::getline(inputStream, rhs);) {
        adjMat[toInt(lhs)][toInt(rhs)] = adjMat[toInt(rhs)][toInt(lhs)] = true;
        vertices.push_back(lhs);
        vertices.push_back(rhs);
        if (lhs > rhs) std::swap(lhs, rhs);
        cliques.insert(lhs + rhs);
    }
    ranges::sort(vertices);
    {
        const auto ret = ranges::unique(vertices);
        vertices.erase(begin(ret), end(ret));
    }
    auto toCliquesVec = [](const std::string& cliq) -> std::vector<std::string> {
        return ranges::to<std::vector>(views::iota(0) | views::stride(2) | views::take(cliq.size() / 2) |
                                       views::transform([&](int i) -> std::string { return cliq.substr(i, 2); }));
    };
    while (true) {
        std::unordered_set<std::string> nextCliques;
        for (const auto& cliq : cliques) {
            const auto cliquesVec = toCliquesVec(cliq);
            std::vector<std::string> diff;
            ranges::set_difference(vertices, cliquesVec, std::back_inserter(diff));
            for (const auto& vert : diff)
                if (ranges::all_of(cliquesVec, [&](const auto& uert) { return adjMat[toInt(vert)][toInt(uert)]; })) {
                    std::set<std::string> newCliqSet(begin(cliquesVec), end(cliquesVec));
                    newCliqSet.insert(vert);
                    std::string newCliq = ranges::to<std::string>(newCliqSet | views::join);
                    nextCliques.insert(newCliq);
                }
        }
        if (nextCliques.empty()) break;
        cliques.swap(nextCliques);
    }
    const auto cliquesVec = toCliquesVec(*begin(cliques));
    return ranges::to<std::string>(cliquesVec | views::join_with(','));
}