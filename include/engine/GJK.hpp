#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "maths/vector.hpp"

// For Centre Of Mass
sf::Vector3f averagePoint(const std::vector<sf::Vector3f>& points) {
    sf::Vector3f sum = {0.0f, 0.0f, 0.0f};
    
    // Take a literal average by adding all the points
    for (int i = 0; i < points.size(); i++) {
        sum += points[i];
    }
    // And dividing by the number of points
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

// Triangle case
void simplex3(sf::Vector3f& a, sf::Vector3f& b, sf::Vector3f& c, sf::Vector3f& d, sf::Vector3f& dir) {
    sf::Vector3f n = crossProduct(b - a, c - a); // Normal vector
    sf::Vector3f AO = -a; // Direction to origin

    // AB closest
    if (dotProduct(crossProduct(b - a, n), AO) > 0) {
        c = a;
        dir = crossProduct(crossProduct(b - a, AO), b - a);
        return;
    }
    // AC closest
    if (dotProduct(crossProduct(n, c - a), AO) > 0) {
        b = a;
        dir = crossProduct(crossProduct(c - a, AO), c - a);
        return;
    }

    if (dotProduct(n, AO) > 0) { // Above triangle
        d = c;
        c = b;
        b = a;
        dir = n;
        return;
    }
    // Below triangle
    d = b;
    b = a;
    dir = -n;
    return;
}

// Tetrahedral case
bool simplex4(sf::Vector3f& a, sf::Vector3f& b, sf::Vector3f& c, sf::Vector3f& d, sf::Vector3f& dir) {
    // Face normals
    sf::Vector3f ABC = crossProduct(b - a, c - a);
    sf::Vector3f ACD = crossProduct(c - a, d - a);
    sf::Vector3f ADB = crossProduct(d - a, b - a);

    sf::Vector3f AO = -a; // Direction to origin

    // Outside ABC
    if (dotProduct(ABC, AO) > 0) {
        d = c;
        c = b;
        b = a;
        dir = ABC;
        return false;
    }
    // Outside ACD
    if (dotProduct(ACD, AO) > 0) {
        b = a;
        dir = ACD;
        return false;
    }
    // Outside ADB
    if (dotProduct(ADB, AO) > 0) {
        c = d;
        d = b;
        b = a;
        dir = ADB;
        return false;
    }

    // Origin in tetrahedron, collision detected
    return true;
}

bool GJK(const std::vector<sf::Vector3f>& p1,
         const std::vector<sf::Vector3f>& p2, sf::Vector3f& mtv) {
    sf::Vector3f a, b, c, d; // 4 Simplex points
    sf::Vector3f dir = averagePoint(p1) - averagePoint(p2); // Search direction

    // Get initial point for simplex
    c = support(p1, p2, dir);
    // Search towards origin
    dir = -c;

    // Get second point for a line segment simplex
    b = support(p1, p2, dir);

    // Check if origin is outside of the simplex
    if (dotProduct(b, dir) < 0) {
        return false; // No collision if not enclosed
    }

    // Search perpendicular to the line segment, towards origin
    dir = crossProduct(crossProduct(c - b, -b), c - b);
    // Check if origin is on the line segment
    if (dir == sf::Vector3f(0, 0, 0)) {
        // Update search direction to be perpendicular to line, normal to x axis
        dir = crossProduct(c - b, sf::Vector3f(1, 0, 0));
        // If somehow origin is still on line, we can guarantee it wont be
        // If we update the search direction to be normal to ¬z axis
        if (dir == sf::Vector3f(0, 0, 0)) {
            dir = crossProduct(c - b, sf::Vector3f(0, 0, -1));
        }
    }

    // Measure of the dimension of the simplex
    int simplexIndex = 2;

    // Perform iterative search
    // 64 is arbitrary - it is a max iteration value for convergence
    for (int i = 0; i < 64; i++) {
        a = support(p1, p2, dir);
        // Check if origin is outside of the simplex
        if (dotProduct(a, dir) < 0) {
            return false; // No collision if not enclosed
        }

        // We add a point to the simplex
        simplexIndex++;
        if (simplexIndex == 3) {
            simplex3(a, b, c, d, dir);
        } else if (simplex4(a, b, c, d, dir)) {
            // mtv = EPA(a, b, c, d, coll1, coll2);
            return true;
        }
    }
    return false;
}