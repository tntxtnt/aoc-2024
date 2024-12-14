#include <day13_solution.h>

#include <vector>
#include <string>
#include <variant>
#include <numeric>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>

template <typename T>
concept Integer = std::is_integral_v<T>;

template <Integer T>
struct Point2i {
    T x;
    T y;
};

template <Integer T>
struct Line2i { // ax + by = c
    T a;
    T b;
    T c;
};

template <Integer T>
static auto intersection(const Line2i<T>& line1,
                         const Line2i<T>& line2) -> std::variant<std::monostate, Point2i<T>, Line2i<T>> {
    const T det = line1.a * line2.b - line1.b * line2.a;
    const T dx = line1.c * line2.b - line1.b * line2.c;
    const T dy = line1.a * line2.c - line1.c * line2.a;
    if (det == 0) {
        if (dx == 0 && dy == 0) return line1;
        return {};
    }
    if (dx % det == 0 && dy % det == 0) return Point2i<T>{dx / det, dy / det};
    return {};
}

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

static auto extractFirstInt(const std::string& str, char prefix) -> int {
    const auto prefixIndex = str.find(prefix);
    return std::stoi(str.substr(prefixIndex + 1, str.find(',') - prefixIndex - 1));
}

static auto extractLastInt(const std::string& str, char prefix) -> int {
    return std::stoi(str.substr(str.find_last_of(prefix) + 1));
}

template <Integer T>
static auto solve(std::istream& inputStream, const T added) -> T {
    T res{};
    for (std::string line1, line2, line3;
         std::getline(inputStream, line1) && std::getline(inputStream, line2) && std::getline(inputStream, line3);
         std::getline(inputStream, line1)) {
        Line2i<T> intLine1{extractFirstInt(line1, '+'), extractFirstInt(line2, '+'),
                           extractFirstInt(line3, '=') + added};
        Line2i<T> intLine2{extractLastInt(line1, '+'), extractLastInt(line2, '+'), extractLastInt(line3, '=') + added};
        std::visit(overloaded{[&](std::monostate) {},                                //
                              [&](const Point2i<T>& pt) { res += 3 * pt.x + pt.y; }, //
                              [&](const Line2i<T>& ln) {
                                  const auto [a, b, c] = ln;
                                  // Note: extended Euclid algorithm is faster
                                  const auto g = std::gcd(a, b);
                                  if (c % g != 0) return;
                                  Point2i<T> p1;
                                  for (p1.x = 0; (c - a * p1.x) % b != 0;) ++p1.x;
                                  p1.y = (c - a * p1.x) / b;
                                  Point2i<T> p2;
                                  for (p2.y = 0; (c - b * p2.y) % a != 0;) ++p2.y;
                                  p2.x = (c - b * p2.y) / a;
                                  res += std::min(3 * p1.x + p1.y, 3 * p2.x + p2.y);
                              }},
                   intersection(intLine1, intLine2));
    }
    return res;
}

auto Day13Solution::part1(std::istream& inputStream) -> Part1ResultType {
    return solve<Part1ResultType>(inputStream, 0);
}

auto Day13Solution::part2(std::istream& inputStream) -> Part2ResultType {
    constexpr long long added = 10'000'000'000'000;
    return solve<Part2ResultType>(inputStream, added);
}