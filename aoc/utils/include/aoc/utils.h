#pragma once

#include <aoc/utils/istream_extract.h>
#include <aoc/utils/linewrapper.h>
#include <aoc/utils/print.h>

#include <string>

namespace aoc
{
template <class T>
auto toString(const T& arg) -> std::string {
    if constexpr (std::is_convertible_v<T, std::string>) {
        return std::string(arg);
    } else {
        using std::to_string;
        return to_string(arg);
    }
}
} // namespace aoc