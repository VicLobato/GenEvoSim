#pragma once
#include <cmath>
#include <vector>

sf::Vector3f operator+(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

sf::Vector3f operator-(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

sf::Vector3f operator*(float scalar, const sf::Vector3f& v) {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

sf::Vector3f operator/(float scalar, const sf::Vector3f& v) {
    return {v.x / scalar, v.y / scalar, v.z / scalar};
}

float dotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

sf::Vector3f crossProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

sf::Vector3f normalise(const sf::Vector3f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (length == 0) {return v;} // Edge case detection

    return sf::Vector3f(v.x / length, v.y / length, v.z / length);
}

float length(const sf::Vector3f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Infinite perpendicular lines in 3D, ouch
sf::Vector3f perpendicular(const sf::Vector3f& v) {
    // Two random non-parallel vectors
    sf::Vector3f v1 = {1.0f, 0.0f, 0.0f};
    sf::Vector3f v2 = {0.0f, 1.0f, 0.0f};

    sf::Vector3f cross = crossProduct(v, v1);

    // Basically - if input vector V is parallel to v1, then we use v2 instead
    if (cross.x == 0.0f && cross.y == 0.0f && cross.z == 0.0f) {
        cross = crossProduct(v, v2);
    }

    return cross;
}