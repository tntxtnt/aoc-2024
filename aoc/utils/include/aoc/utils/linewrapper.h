#pragma once

#include <string>
#include <istream>

struct LineWrapper {
    std::string line;
    operator std::string() const&;
    operator std::string() &&;
};

std::istream& operator>>(std::istream& is, LineWrapper& lineWrapper);