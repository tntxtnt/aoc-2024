#pragma once

#include <istream>

class Day11Solution {
public:
    using Part1ResultType = long long;
    using Part2ResultType = long long;

    auto part1(std::istream& inputStream, int blinkCount) -> Part1ResultType;
    auto part2(std::istream& inputStream) -> Part2ResultType;

private:
};