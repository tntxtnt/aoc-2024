#include <day22_solution.h>

#include <vector>
#include <string>
#include <array>
#include <ranges>
namespace ranges = std::ranges;
namespace views = std::views;
#include <aoc/utils.h>
#include <Eigen/Dense>

struct Bool {
    bool value{};
    Bool() = default;
    Bool(int val) : value{(bool)val} {}
    auto operator+=(Bool rhs) -> Bool& {
        value ^= rhs.value;
        return *this;
    }
    auto operator+(Bool rhs) const -> Bool { return Bool{value ^ rhs.value}; }
    auto operator*=(Bool rhs) -> Bool& {
        value &= rhs.value;
        return *this;
    }
    auto operator*(Bool rhs) const -> Bool { return Bool{value & rhs.value}; }
    auto operator==(Bool rhs) const -> bool { return value == rhs.value; }
};

using Matrix24b = Eigen::Matrix<Bool, 24, 24>;
using Vector24b = Eigen::Matrix<Bool, 1, 24>;

auto getMatrixT() -> Matrix24b {
    Matrix24b L6;
    Matrix24b R5;
    Matrix24b L11;
    L6.bottomLeftCorner(18, 18) = Eigen::Matrix<Bool, 18, 18>::Identity();
    R5.topRightCorner(19, 19) = Eigen::Matrix<Bool, 19, 19>::Identity();
    L11.bottomLeftCorner(13, 13) = Eigen::Matrix<Bool, 13, 13>::Identity();
    const auto I = Matrix24b::Identity();
    return (I + L6) * (I + R5) * (I + L11);
}

auto toVec(unsigned num) -> Vector24b {
    Vector24b res;
    for (size_t i = 24; i-- > 0;) {
        res[i] = num & 1;
        num >>= 1;
    }
    return res;
}

auto fromVec(const Vector24b& vec) -> unsigned {
    unsigned res{};
    for (size_t i = 0; i < 24; ++i) res = res * 2 + (int)vec[i].value;
    return res;
}

auto pow(Matrix24b mat, size_t exp) -> Matrix24b {
    auto advance = [](auto& exp, auto& mat) {
        exp >>= 1;
        mat *= mat;
    };
    while (exp % 2 != 1) advance(exp, mat);
    Matrix24b res = mat;
    for (advance(exp, mat); exp > 0; advance(exp, mat))
        if (exp % 2 == 1) res *= mat;
    return res;
}

struct Xorshift24 {
    using value_type = unsigned;
    value_type seed : 24 {}; // make seed 24-bit = part 1 runs slower from 8.8ms to 11.3ms with MSVC??
    auto random() -> value_type {
        seed ^= seed << 6;
        seed ^= seed >> 5;
        seed ^= seed << 11;
        return seed;
    }
};

constexpr unsigned changeCount = 2000;
static const auto T2000 = pow(getMatrixT(), changeCount);

auto Day22Solution::part1(std::istream& inputStream) -> Part1ResultType {
    Part1ResultType res{};
    for (unsigned num : views::istream<unsigned>(inputStream)) {
        // Xorshift24 gen{num};
        // ranges::for_each(views::iota(0) | views::take(changeCount - 1), [&](int /*unused*/) { gen.random(); });
        // res += gen.random();
        res += fromVec(toVec(num) * T2000); // 1.5ms
    }
    return res;
}

consteval auto powi(unsigned num, unsigned exp) -> unsigned {
    unsigned res{1};
    for (; exp > 0; exp >>= 1, num *= num)
        if (exp % 2 == 1) res *= num;
    return res;
}

auto Day22Solution::part2(std::istream& inputStream) -> Part2ResultType {
    constexpr unsigned patternSize = 4;
    constexpr unsigned base = 10;
    constexpr unsigned deltaMagnitude = base - 1;
    constexpr unsigned deltaSize = 2 * deltaMagnitude + 1;
    constexpr unsigned hashSize = powi(deltaSize, patternSize);
    std::vector<int> bigMap(hashSize);
    for (Xorshift24::value_type num : views::istream<unsigned>(inputStream)) {
        Xorshift24 gen{num};
        decltype(bigMap) map(hashSize, -1);
        for (unsigned hash{}, prev{num % base}; unsigned i : views::iota(1) | views::take(changeCount)) {
            unsigned curr = gen.random() % base;
            hash = (hash * deltaSize + curr - prev + deltaMagnitude) % hashSize;
            if (i >= patternSize && map[hash] == -1) map[hash] = (int)curr;
            prev = curr;
        }
        for (auto [key, val] : views::enumerate(map))
            if (val != -1) bigMap[key] += val;
    }
    return ranges::max(bigMap);
}