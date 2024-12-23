#include <day23_solution.h>

#include <vector>
#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>
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

auto toInt(std::string_view str) -> int {
    return (str[0] - 'a') * 26 + str[1] - 'a';
};

auto fromInt(int num) -> std::string {
    std::string res(2, '\0');
    res[0] = (char)(num / 26 + 'a');
    res[1] = (char)(num % 26 + 'a');
    return res;
};

template <class T>
inline void hash_combine(size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct Clique {
    const std::vector<std::vector<bool>>& adj;
    std::vector<int> candidates;
    std::vector<int> verts;

    Clique(int vt1, int vt2, const std::vector<int>& candies, const std::vector<std::vector<bool>>& adjMat)
    : adj{adjMat}, candidates{}, verts{vt1, vt2} {
        ranges::sort(verts);
        ranges::copy_if(candies, std::back_inserter(candidates),
                        [&](int vert) { return adj[vert][vt1] && adj[vert][vt2]; });
    }
    auto operator==(const Clique& rhs) const { return verts == rhs.verts; }
    auto to_string() const -> std::string {
        return ranges::to<std::string>(verts | views::transform(fromInt) | views::join_with(','));
    }
    template <class Func>
    void forEachLargerBy1Clique(Func&& func) const {
        for (int newVert : candidates) std::forward<Func>(func)(*this + newVert);
    }

private:
    Clique(const std::vector<std::vector<bool>>& adjMat) : adj{adjMat} {}
    auto operator+(int newVert) const -> Clique {
        Clique res{adj};
        // verts
        res.verts.reserve(verts.size() + 1);
        ranges::copy(verts | views::take_while([=](int vert) { return vert < newVert; }),
                     std::back_inserter(res.verts));
        res.verts.push_back(newVert);
        ranges::copy(verts | views::drop(res.verts.size() - 1), std::back_inserter(res.verts));
        // candidates
        size_t newCandidatesSize = ranges::count_if(candidates, [&](int vert) { return adj[vert][newVert]; });
        res.candidates.reserve(newCandidatesSize);
        ranges::copy_if(candidates, std::back_inserter(res.candidates), [&](int vert) { return adj[vert][newVert]; });
        return res;
    }
};
struct CliqueHash {
    auto operator()(const Clique& cliq) const -> size_t {
        size_t res{};
        for (int num : cliq.verts) hash_combine(res, num);
        return res;
    }
};

auto Day23Solution::part2(std::istream& inputStream) -> Part2ResultType {
    std::vector<std::vector<bool>> adjMat(26 * 26, std::vector<bool>(26 * 26, false));
    std::vector<int> vertices;
    std::vector<std::pair<int, int>> edges;
    for (std::string lhs, rhs; std::getline(inputStream, lhs, '-') && std::getline(inputStream, rhs);) {
        const int vt1 = toInt(lhs);
        const int vt2 = toInt(rhs);
        adjMat[vt1][vt2] = adjMat[vt2][vt1] = true;
        vertices.push_back(vt1);
        vertices.push_back(vt2);
        edges.emplace_back(vt1, vt2);
    }
    ranges::sort(vertices);
    {
        const auto ret = ranges::unique(vertices);
        vertices.erase(begin(ret), end(ret));
    }

    std::unordered_set<Clique, CliqueHash> cliques;
    for (auto [vt1, vt2] : edges) cliques.emplace(vt1, vt2, vertices, adjMat);
    while (true) {
        decltype(cliques) nextCliques;
        for (const auto& cliq : cliques)
            cliq.forEachLargerBy1Clique([&](Clique newClique) { nextCliques.insert(std::move(newClique)); });
        if (nextCliques.empty()) break;
        cliques.swap(nextCliques);
    }
    return begin(cliques)->to_string();
}