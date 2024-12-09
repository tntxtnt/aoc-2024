#include <day07_solution.h>
#include <sstream>
#include <vector>
#include <span>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

auto Day7Solution::part1(std::istream& is) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string line : views::istream<LineWrapper>(is)) {
        std::istringstream iss{line};
        long long target{};
        iss >> target;
        iss.ignore();
        auto nums = ranges::to<std::vector<long long>>(views::istream<long long>(iss));
        auto canBeEvalTo = [&](this auto&& canBeEvalTo, std::span<const long long> nums, long long total) -> bool {
            if (total > target) return false;
            if (nums.empty()) return total == target;
            return canBeEvalTo(nums | views::drop(1), total + nums[0]) ||
                   canBeEvalTo(nums | views::drop(1), total * nums[0]);
        };
        if (canBeEvalTo(nums | views::drop(1), nums[0])) res += target;
    }
    return res;
}

auto Day7Solution::part2(std::istream& is) -> Part2ResultType {
    Part2ResultType res{};
    for (std::string line : views::istream<LineWrapper>(is)) {
        std::istringstream iss{line};
        long long target{};
        iss >> target;
        iss.ignore();
        auto nums = ranges::to<std::vector<long long>>(views::istream<long long>(iss));
        const auto concat = [](long long a, long long b) { return std::stoll(fmt::format("{}{}", a, b)); };
        auto canBeEvalTo = [&](this auto&& canBeEvalTo, std::span<const long long> nums, long long total) -> bool {
            if (total > target) return false;
            if (nums.empty()) return total == target;
            return canBeEvalTo(nums | views::drop(1), total + nums[0]) ||
                   canBeEvalTo(nums | views::drop(1), total * nums[0]) ||
                   canBeEvalTo(nums | views::drop(1), concat(total, nums[0]));
        };
        if (canBeEvalTo(nums | views::drop(1), nums[0])) res += target;
    }
    return res;
}