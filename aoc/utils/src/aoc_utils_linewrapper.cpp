#include <aoc/utils/linewrapper.h>

LineWrapper::operator std::string() const& {
    return line;
}

LineWrapper::operator std::string() && {
    return std::move(line);
}

std::istream& operator>>(std::istream& is, LineWrapper& lineWrapper) {
    return std::getline(is, lineWrapper.line);
}