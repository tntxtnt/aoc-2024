#pragma once

#include <istream>

class Day14Solution {
public:
    using Part1ResultType = int;
    using Part2ResultType = int;

    auto part1(std::istream& is, const int width, const int height) -> Part1ResultType;
    auto part2(std::istream& is, const int width, const int height) -> Part2ResultType;

private:
};