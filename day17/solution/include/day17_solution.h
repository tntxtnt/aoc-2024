#pragma once

#include <istream>
#include <string>

class Day17Solution {
public:
    using Part1ResultType = std::string;
    using Part2ResultType = long long;

    auto part1(std::istream& inputStream) -> Part1ResultType;
    auto part2(std::istream& inputStream) -> Part2ResultType;

private:
};