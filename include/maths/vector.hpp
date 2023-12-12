#pragma once
#include <cmath>
#include <vector>

// Adding two vectors
sf::Vector3f operator+(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// Subtracting two vectors
sf::Vector3f operator-(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// Multiplying a vector by a scalar
sf::Vector3f operator*(float scalar, const sf::Vector3f& v) {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

// Divide by a scalar
sf::Vector3f operator/(float scalar, const sf::Vector3f& v) {
    return {v.x / scalar, v.y / scalar, v.z / scalar};
}

// Dot product of two vectors
float dotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Cross product two vectors
sf::Vector3f crossProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

// Return length using pythagoras
float length(const sf::Vector3f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Normalise vector
// Means keep ratio of values the same
// But length is 1
sf::Vector3f normalise(const sf::Vector3f& v) {
    float len = length(v);

    if (len == 0) {return v;} // Edge case detection

    return sf::Vector3f(v.x / len, v.y / len, v.z / len);
}