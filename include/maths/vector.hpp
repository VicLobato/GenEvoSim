#pragma once
#include <iostream> 
#include <vector>

struct Edge3D {
    sf::Vector3f normal;
    float distance;
};

struct Polygon3D {
    std::vector<sf::Vector3f> vertices;
};

sf::Vector3f operator-(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

sf::Vector3f operator*(float scalar, const sf::Vector3f& v) {
    return {scalar * v.x, scalar * v.y, scalar * v.z};
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

Edge3D getEdge(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    sf::Vector3f edgeVector = v2 - v1;
    sf::Vector3f edgeNormal = crossProduct(edgeVector, {0, 0, 1});
    float edgeDistance = dotProduct(edgeNormal, v1);
    return {edgeNormal, edgeDistance};
}