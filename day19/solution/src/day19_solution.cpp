#include <day19_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

auto getTowelPatterns(std::istream& inputStream) -> std::vector<std::string> {
    std::vector<std::string> towelPatterns;
    std::string line;
    std::getline(inputStream, line);
    std::istringstream iss{line};
    while (iss >> line) {
        if (line.back() == ',') line.pop_back();
        towelPatterns.push_back(line);
    }
    return towelPatterns;
}

auto Day19Solution::part1(std::istream& inputStream) -> Part1ResultType {
    const auto towelPatterns = getTowelPatterns(inputStream);
    Part1ResultType res{};
    for (std::string design; inputStream >> design;) {
        auto canBeMade = [&](this auto&& canBeMade, std::string_view dsg) -> bool {
            if (dsg.empty()) return true;
            for (auto& pat : towelPatterns)
                if (dsg.starts_with(pat) && canBeMade(dsg.substr(pat.size()))) return true;
            return false;
        };
        res += (int)canBeMade(design);
    }
    return res;
}

auto Day19Solution::part2(std::istream& inputStream) -> Part2ResultType {
    const auto towelPatterns = getTowelPatterns(inputStream);
    Part2ResultType res{};
    for (std::string design; inputStream >> design;) {
        std::vector<Part2ResultType> cache(design.size() + 1, -1);
        auto canBeMade = [&](this auto&& canBeMade, std::string_view dsg) -> Part2ResultType {
            if (cache[dsg.size()] != -1) return cache[dsg.size()];
            if (dsg.empty()) return 1;
            Part2ResultType cnt{};
            for (auto& pat : towelPatterns)
                if (dsg.starts_with(pat)) cnt += canBeMade(dsg.substr(pat.size()));
            return cache[dsg.size()] = cnt;
        };
        canBeMade(design);
        res += cache.back();
    }
    return res;
}