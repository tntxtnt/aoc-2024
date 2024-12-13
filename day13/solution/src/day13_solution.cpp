#include <day13_solution.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

auto Day13Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Part1ResultType res{};
    for (std::string line1, line2, line3;
         std::getline(inputStream, line1) && std::getline(inputStream, line2) && std::getline(inputStream, line3);) {
        // Ax + By = C
        // Dx + Ey = F
        const int a = std::stoi(line1.substr(line1.find('+') + 1, line1.find(',') - line1.find('+') + 1));
        const int b = std::stoi(line2.substr(line2.find('+') + 1, line2.find(',') - line2.find('+') + 1));
        const int c = std::stoi(line3.substr(line3.find('=') + 1, line3.find(',') - line3.find('=') + 1));
        // aoc::println("{}x + {}y = {}", a, b, c);
        const int d = std::stoi(line1.substr(line1.find_last_of('+') + 1));
        const int e = std::stoi(line2.substr(line2.find_last_of('+') + 1));
        const int f = std::stoi(line3.substr(line3.find_last_of('=') + 1));
        // aoc::println("{}x + {}y = {}", d, e, f);
        // det = AE - BD
        // x = (CE - BF) / det
        // y = (AF - CD) / det
        const int det = a * e - b * d;
        const int dx = c * e - b * f;
        const int dy = a * f - c * d;
        if (det == 0) {
            // dx == dy == 0 or no solution
            if (dx == 0 && dy == 0) { // AoC input doesn't contain such cases
                // Ax + By = C
                //    x = (C - By) / A
                // or y = (C - Ax) / B
                int minRes{101 * 3 + 101};
                int solX{};
                int solY{};
                for (int y = 1, n = c - b; y <= 100 && n >= 0; ++y, n -= b) {
                    if (n % a == 0) {
                        const int val = 3 * (n / a) + y;
                        if (val < minRes) {
                            minRes = val;
                            solX = n / a;
                            solY = y;
                        }
                    }
                }
                for (int x = 1, n = c - a; x <= 100 && n >= 0; ++x, n -= a) {
                    if (n % b == 0) {
                        const int val = 3 * x + n / b;
                        if (val < minRes) {
                            minRes = val;
                            solX = x;
                            solY = n / b;
                        }
                    }
                }
                aoc::println("{}x + {}y = {}", a, b, c);    // these lines
                aoc::println("{}x + {}y = {}", d, e, f);    // are
                aoc::println("x = {}, y = {}", solX, solY); // never getting printed
                res += minRes;
            }
        } else {
            if (dx % det == 0 && dy % det == 0) {
                const int x = dx / det;
                const int y = dy / det;
                // aoc::println("x = {}, y = {}", x, y);
                res += 3 * x + y;
            }
        }
        std::getline(inputStream, line1); // ignore empty line
    }
    return res;
}

auto Day13Solution::part2(std::istream& inputStream) -> Part2ResultType {
    constexpr long long added = 10'000'000'000'000;
    Part2ResultType res{};
    for (std::string line1, line2, line3;
         std::getline(inputStream, line1) && std::getline(inputStream, line2) && std::getline(inputStream, line3);) {
        // Ax + By = C
        // Dx + Ey = F
        const long long a = std::stoi(line1.substr(line1.find('+') + 1, line1.find(',') - line1.find('+') + 1));
        const long long b = std::stoi(line2.substr(line2.find('+') + 1, line2.find(',') - line2.find('+') + 1));
        const long long c = std::stoi(line3.substr(line3.find('=') + 1, line3.find(',') - line3.find('=') + 1)) + added;
        // aoc::println("{}x + {}y = {}", a, b, c);
        const long long d = std::stoi(line1.substr(line1.find_last_of('+') + 1));
        const long long e = std::stoi(line2.substr(line2.find_last_of('+') + 1));
        const long long f = std::stoi(line3.substr(line3.find_last_of('=') + 1)) + added;
        // aoc::println("{}x + {}y = {}", d, e, f);
        // det = AE - BD
        // x = (CE - BF) / det
        // y = (AF - CD) / det
        const long long det = a * e - b * d;
        const long long dx = c * e - b * f;
        const long long dy = a * f - c * d;
        if (det == 0) {
            // dx == dy == 0 or no solution
            if (dx == 0 && dy == 0) {
                // Ax + By = C
                //    x = (C - By) / A
                // or y = (C - Ax) / B
                // don't know how to solve this
                // ignore since AoC input doesn't contain such cases
                aoc::println("{}x + {}y = {}", a, b, c);
                aoc::println("{}x + {}y = {}", d, e, f);
            }
        } else {
            if (dx % det == 0 && dy % det == 0) {
                const long long x = dx / det;
                const long long y = dy / det;
                // aoc::println("x = {}, y = {}", x, y);
                res += 3 * x + y;
            }
        }
        std::getline(inputStream, line1); // ignore empty line
    }
    return res;
}