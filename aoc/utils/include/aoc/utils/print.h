#pragma once

#include <stdexcept>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <nowide/iostream.hpp>

namespace aoc
{
template <typename... T>
void println(fmt::format_string<T...> fmt, T&&... args) {
    fmt::println(nowide::cout, fmt, static_cast<T&&>(args)...);
    nowide::cout.flush();
}

template <typename... T>
void print(fmt::format_string<T...> fmt, T&&... args) {
    fmt::print(nowide::cout, fmt, static_cast<T&&>(args)...);
}

template <typename... T>
void perror(fmt::format_string<T...> fmt, T&&... args) {
    const std::string errorMessage = fmt::format(fmt, static_cast<T&&>(args)...);
    fmt::println(nowide::cerr, "{}", fmt::styled(errorMessage, fg(fmt::color::red)));
}

template <typename T>
void println(T&& arg) {
    fmt::println(nowide::cout, "{}", static_cast<T&&>(arg));
    nowide::cout.flush();
}

template <typename T>
void print(T&& arg) {
    fmt::print(nowide::cout, "{}", static_cast<T&&>(arg));
}

template <typename T>
void perror(const std::exception& ex) {
    const std::string errorMessage = fmt::format("Error: {}", ex.what());
    fmt::println(nowide::cerr, "{}", fmt::styled(errorMessage, fg(fmt::color::red)));
}
} // namespace aoc