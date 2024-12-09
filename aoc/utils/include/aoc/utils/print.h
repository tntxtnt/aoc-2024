#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/color.h>
#include <nowide/iostream.hpp>

namespace aoc
{
template <typename... T>
void println(fmt::format_string<T...> fmt, T&&... args) {
    return fmt::println(nowide::cout, fmt, static_cast<T&&>(args)...);
}

template <typename... T>
void print(fmt::format_string<T...> fmt, T&&... args) {
    return fmt::print(nowide::cout, fmt, static_cast<T&&>(args)...);
}

template <typename... T>
void perror(fmt::format_string<T...> fmt, T&&... args) {
    const std::string errorMessage = fmt::format(fmt, static_cast<T&&>(args)...);
    return fmt::println(nowide::cerr, "{}", fmt::styled(errorMessage, fg(fmt::color::red)));
}
} // namespace aoc