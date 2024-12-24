#pragma once

#include <istream>

class Day24Solution {
public:
    using Part1ResultType = unsigned long long;
    using Part2ResultType = std::string;

    auto part1(std::istream& inputStream) -> Part1ResultType;
    auto part2(std::istream& inputStream) -> Part2ResultType;

private:
};