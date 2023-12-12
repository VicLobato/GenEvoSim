#pragma once
#include "maths/funcs.hpp"

std::string round(auto value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}