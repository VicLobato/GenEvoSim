#pragma once

std::string round(auto value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

float clamp(float value, float low, float high) { 
    if (value < low) {return low;}
    if (value > high) {return high;}
    else return value;
}