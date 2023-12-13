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

// Find furthest point along direction vector
int furthestAlong(const std::vector<sf::Vector3f>& points, sf::Vector3f dir) {
    // Start with the first points distance
    float maxDist = dotProduct(dir, points[0]);
    int index = 0;
    // Iterate over every point
    for (int i = 1; i < points.size(); i++) {
        float product = dotProduct(dir, points[i]);
        if (product > maxDist) { // If new point is further, update distance and index
            maxDist = product;
            index = i;
        }
    }
    return index;
}

// Support function
sf::Vector3f support(const std::vector<sf::Vector3f>& p1,
                     const std::vector<sf::Vector3f>& p2, sf::Vector3f d) {
    
    // Find furthest point along opposing vectors for each set
    int i = furthestAlong(p1,  d);
    int j = furthestAlong(p2, -d);

    // Minkowski difference
    return (p1[i] - p2[j]);
}