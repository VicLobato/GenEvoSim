#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "maths/vector.hpp"

// For Centre Of Mass
sf::Vector3f averagePoint(const std::vector<sf::Vector3f>& points) {
    sf::Vector3f sum = {0.0f, 0.0f, 0.0f};
    
    for (int i = 0; i < points.size(); i++) {
        sum += points[i];
    }

    return sum / static_cast<float>(points.size());
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

// GJK Algorithm
bool GJK(const std::vector<sf::Vector3f>& p1,
         const std::vector<sf::Vector3f>& p2) {
    int simplexIndex = 0;
    sf::Vector3f x, y, z, xo, xy, xz, xyP, xzP, d, simplex[3]; // P indicates perpendicular

    // Technically not Centre Of Mass
    // But also it is, due to assumed uniform density
    sf::Vector3f pos1 = averagePoint(p1);
    sf::Vector3f pos2 = averagePoint(p2);

    // Initial direction between centres
    d = pos1 - pos2;

    // Edge case: zero direction value - set to random direction, in this case (1, 0, 0)
    if (d.x == 0 && d.y == 0 && d.z == 0) {
        d.x = 1.0f;
    }

    // First support set as intial point
    x = simplex[0] = support(p1, p2, d);

    // Collision check
    if (dotProduct(x, d) <= 0) {
        return false;
    }

    d = -x; // Invert direction to go toward origin

    // GJK is iterative - it converges on a solution
    int iter = 0;

    while (true) {
        iter++;

        x = simplex[++simplexIndex] = support(p1, p2, d); // Preincrement operation, new support

        // Collision check
        if (dotProduct(x, d) <= 0) {
            return false;
        }

        // Begin constructing 2D simplex
        xo = -x;

        // Perform linear checks
        if (simplexIndex < 2) {
            y = simplex[0];
            xy = y - x; // Direction vector from x to y
            d = tripleProduct(xy, xo, xy); // XY normal towards origin

            if (length(d) == 0) {
                d = perpendicular(xy);
            }
            continue; // Skip current iteration
        }

        // Construct 3D simplex
        y = simplex[1];
        z = simplex[0];
        xy = y - x; // Direction vector from x to y
        xz = z - x; // Direction vector from x to z

        // Perpendicular line
        xzP = tripleProduct(xy, xz, xz);

        if (dotProduct(xzP, xo) >= 0) {
            d = xzP; // XZ normal towards origin
        } else {
            xyP = tripleProduct(xz, xy, xy);

            // Check if origin inside simplex
            if (dotProduct(xyP, xo) < 0) {
                return true; // Collision found
            }

            simplex[0] = simplex[1]; // Swap
            d = xyP; // XY normal towards origin
        }

        simplex[1] = simplex[2]; // Swap
        simplexIndex--;
    }

    return false; // No collision :(
}