#include <day07_solution.h>

#include <sstream>
#include <vector>
#include <span>
#include <ranges>
namespace views = std::views;
#include <fmt/format.h>

auto Day7Solution::part1(std::istream& is) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string line; std::getline(is, line);) {
        std::istringstream iss{line};
        long long target{};
        std::vector<long long> nums;
        iss >> target;
        iss.ignore();
        for (long long n; iss >> n;) nums.push_back(n);
        auto canBeEvalTo = [&](this auto&& canBeEvalTo, std::span<long long> nums, long long total) -> bool {
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
    for (std::string line; std::getline(is, line);) {
        std::istringstream iss{line};
        long long target{};
        std::vector<long long> nums;
        iss >> target;
        iss.ignore();
        for (long long n; iss >> n;) nums.push_back(n);
        auto concat = [](long long a, long long b) { return std::stoll(fmt::format("{}{}", a, b)); };
        auto canBeEvalTo = [&](this auto&& canBeEvalTo, std::span<long long> nums, long long total) -> bool {
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