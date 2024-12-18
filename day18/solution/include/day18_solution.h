#pragma once

#include <istream>

class Day18Solution {
public:
    using Part1ResultType = int;
    using Part2ResultType = std::string;

    auto part1(std::istream& inputStream, int width, int height, int wallCount) -> Part1ResultType;
    auto part2(std::istream& inputStream, int width, int height, int wallCount) -> Part2ResultType;

private:
};