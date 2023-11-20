#pragma once
#include <vector>
#include <cmath>
#include "cube.hpp"
#include "maths/vector.hpp"

// Collision detection works through separating axis theorem
bool isOverlap(const Edge3D& edge, const Polygon3D& polygon) {
    float minProjection = dotProduct(edge.normal, polygon.vertices[0]);
    float maxProjection = minProjection;

    for (const auto& vertex : polygon.vertices) {
        float projection = dotProduct(edge.normal, vertex);
        if (projection < minProjection) {
            minProjection = projection;
        } else if (projection > maxProjection) {
            maxProjection = projection;
        }
    }

    return (maxProjection >= edge.distance) && (minProjection <= edge.distance);
}

// Collision can only be found between planar faces - must do x6 per cube, once per face
bool isCollidingFaces(const Polygon3D& polygon1, const Polygon3D& polygon2) {
    for (int i = 0; i < polygon1.vertices.size(); ++i) {
        int nextIndex = (i + 1) % polygon1.vertices.size();
        Edge3D edge = getEdge(polygon1.vertices[i], polygon1.vertices[nextIndex]);
        if (!isOverlap(edge, polygon2)) {
            return false;
        }
    }

    for (int i = 0; i < polygon2.vertices.size(); ++i) {
        int nextIndex = (i + 1) % polygon2.vertices.size();
        Edge3D edge = getEdge(polygon2.vertices[i], polygon2.vertices[nextIndex]);
        if (!isOverlap(edge, polygon1)) {
            return false;
        }
    }

    return true;
}

bool isColliding(const Cube& cube1, const Cube& cube2) {
    std::vector<int> indices = {
        2,3,1,0,
        4,5,7,6,
        0,1,5,4,
        3,2,6,7,
        4,6,2,0,
        1,3,7,5
    };

    std::vector<sf::Vector3f> points1 = cube1.points();
    std::vector<sf::Vector3f> points2 = cube2.points();

    bool intersect = false;

    // Iterate over quads
    for (int i=0; i<24; i+=4) {
            Polygon3D polygon1;
            polygon1.vertices = {
                points1[indices[i  ]],
                points1[indices[i+1]],
                points1[indices[i+2]],
                points1[indices[i+3]]
            };
            Polygon3D polygon2;
            polygon2.vertices = {
                points2[indices[i  ]],
                points2[indices[i+1]],
                points2[indices[i+2]],
                points2[indices[i+3]]
            };
        intersect = intersect || isCollidingFaces(polygon1, polygon2);
        if (intersect == true) {return true;}
    }
    // instead of rechecking whether the last loop returned true or false, we can just default return its value
    return intersect;
}

void collisionDetection(const std::vector<Cube> objs) {
    // perform pairwise collision detection
    int numObjs = objs.size();

    for (int i = 0; i < numObjs; ++i) {
        for (int j = i + 1; j < numObjs; ++j) {
            if (isColliding(objs[i], objs[j])) {
                std::cout << std::to_string(i) << " " << std::to_string(j) << " colliding\n";
            }
        }
    }
}