#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "cube.hpp"
#include "engine/GJK.hpp"
#include "maths/funcs.hpp"
#include "maths/vector.hpp"

// Perform GJK using the absolute coordinates
bool colliding(const Cube& c1, const Cube& c2) {
    return GJK(c1.points(), c2.points());
}

// Runs collision detection on all cubes
void collisionDetection(const std::vector<Cube>& cubes) {
    // Pairwise collision detection
    for (int i = 0; i < cubes.size(); i++) {
        for (int j = i + 1; j < cubes.size(); j++) {
            if (colliding(cubes[i], cubes[j])) {
                std::cout << "Colliding cubes: " << i << " and " << j << std::endl;
            }
        }
    }
}