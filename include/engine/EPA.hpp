#pragma once
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "maths/vector.hpp"

#define EPA_TOLERANCE 0.01
#define EPA_MAX_ITERATION 64

sf::Vector3f EPA(sf::Vector3f a, sf::Vector3f b, sf::Vector3f c, sf::Vector3f d, const std::vector<sf::Vector3f>& p1, const std::vector<sf::Vector3f>& p2) {
    // Reconstruct the simplex from GJK
    // good starting point as it contains the origin
    // Faces are ABC, ACD, ADB, BDC
    std::vector<sf::Vector3f> simplex;
    simplex.push_back(a);
    simplex.push_back(b);
    simplex.push_back(c);
    simplex.push_back(normalise(crossProduct(b - a, c - a)));
    simplex.push_back(a);
    simplex.push_back(c);
    simplex.push_back(d);
    simplex.push_back(normalise(crossProduct(c - a, d - a)));
    simplex.push_back(a);
    simplex.push_back(b);
    simplex.push_back(d);
    simplex.push_back(normalise(crossProduct(d - a, b - a)));
    simplex.push_back(b);
    simplex.push_back(c);
    simplex.push_back(d);
    simplex.push_back(normalise(crossProduct(b - d, c - d)));

    int closestFace = 0;
    float dist = 0;

    for (int iter = 0; iter < EPA_MAX_ITERATION; iter++) {
        // Find face closest to origin
        float closestDistance = dotProduct(simplex[0], simplex[3]); // Set to distance of 1st one
        for (int i = 4; i < simplex.size(); i += 4) {
            float distance = dotProduct(simplex[i], simplex[i + 3]); 
            if (distance < closestDistance) {
                closestDistance = distance;
                closestFace = i;
            }
        }

        // Generate new support direction along face normal and check
        sf::Vector3f dir = support(p1, p2, simplex[closestFace + 3]);
        dist = dotProduct(dir, simplex[closestFace + 3]);
        if (dist - closestDistance < EPA_TOLERANCE) {
            // Converged, return vector
            return simplex[closestFace + 3] * dist;
        }

        
    }

    // We exit loop if we didn't converge
    // Return closest vector
    return simplex[closestFace + 3] * dist; 
}