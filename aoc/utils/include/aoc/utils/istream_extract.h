#pragma once

#include <istream>
#include <utility>

template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;
};

template <class T1, class T2>
std::istream& operator>>(std::istream& is, Pair<T1, T2>& p) {
    return is >> p.first >> p.second;
}