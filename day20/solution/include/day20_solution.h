#pragma once

#include <istream>

class Day20Solution {
public:
    using Part1ResultType = int;
    using Part2ResultType = int;

    auto part1(std::istream& inputStream, int leastPicosecondSaved) -> Part1ResultType;
    auto part2(std::istream& inputStream, int leastPicosecondSaved) -> Part2ResultType;

private:
};