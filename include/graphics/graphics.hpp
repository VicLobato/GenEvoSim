#pragma once
#include "matrix.hpp"
#include "cube.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <cmath>

float distance(const sf::Vector3f a, const sf::Vector3f b) {
    const float x = a.x - b.x;
    const float y = a.y - b.y;
    const float z = a.z - b.z;
    return std::sqrt(x*x + y*y + z*z);
}

class Camera {
    public:
        sf::Vector3f position = {0, 0, 0};
        float xRotation = 0; // Like a magnetic bearing
        float yRotation = 0; // I gave up, I'm now using euler angles
        float zRotation = 0; // ^ See above
        float fov = 45; // In degrees
        int CLIP_DISTANCE = 1000;

        std::vector<Cube> objs; // List of world objects
        sf::RenderWindow *window; // Pointer reference to SFML window for rendering

        // Projection matrices - precalculated to save on time
        Matrix rotationX;
        Matrix rotationY;
        Matrix rotationZ; 

    Camera(sf::RenderWindow *_window) : window(_window) {}

    // void project(sf::Vector3f point) {
    //     a
    // }

    void recalculate() {
        // Precalculate rotation values
        float sinX = std::sin(-xRotation);
        float cosX = std::cos(-xRotation);
        float sinY = std::sin( yRotation);
        float cosY = std::cos( yRotation);
        float sinZ = std::sin( zRotation);
        float cosZ = std::cos( zRotation);

        // https://en.wikipedia.org/wiki/3D_projection#Mathematical_formula
        rotationX.data = {{1, 0, 0}, {0, cosX, sinX}, {0, -sinX, cosX}};
        rotationY.data = {{cosY, 0, -sinY}, {0, 1, 0}, {sinY, 0, cosY}};
        rotationZ.data = {{cosZ, sinZ, 0}, {-sinZ, cosZ, 0}, {0, 0, 1}};
    }

    void render() {
        recalculate();
        // Iterate over the cubes and draw them
        for (const auto &cube : objs) {
            std::vector<sf::Vector3f> absoluteCoords = cube.points(); // Recalculate absolute points

            // The 6 faces and the constituent corners
            std::vector<std::vector<int>> polygonIndices = {
                {0, 1, 3, 2},
                {4, 5, 7, 6},
                {3, 1, 5, 7},
                {2, 0, 4, 6},
                {5, 1, 0, 4},
                {2, 3, 7, 6}
            };

            float depth = 1 / std::tan(fov * 3.14159 / 360);
            std::vector<sf::Vector3f> coords; // The corners used for screen-space
            for (const auto& coord : absoluteCoords) {
                Matrix coordMatrix(3, 1);
                coordMatrix.data = {{
                    coord.x - position.x,
                    coord.y - position.y,
                    coord.z - position.z
                }};

                Matrix rotatedCoord = rotationX * rotationY * rotationZ * coordMatrix;
                coords.push_back({
                    depth * rotatedCoord.data[0][0] / rotatedCoord.data[2][0],
                    depth * rotatedCoord.data[1][0] / rotatedCoord.data[2][0],
                    rotatedCoord.data[2][0]
                });
            }

            // Map to screen-space coordinates
            // Coords are being returned as -1,-1 to 1,1
            // We need it to go from 0,0 to width,height
            if ((*window).getSize().x > (*window).getSize().y) {
                for (auto &coord : coords) {
                    coord.x += ( coord.x + 1) * (*window).getSize().y / 2 + ((*window).getSize().x -  (*window).getSize().y) / 2;
                    coord.y += (-coord.y + 1) * (*window).getSize().y / 2;
                }
            } else {
                for (auto &coord : coords) {
                    coord.x += ( coord.x + 1) * (*window).getSize().x / 2;
                    coord.y += (-coord.y + 1) * (*window).getSize().x / 2 + ((*window).getSize().y -  (*window).getSize().x) / 2;
                }
            }

            // Render using SFML
            for (const auto &indices : polygonIndices) {
                sf::ConvexShape polygon;
                polygon.setFillColor(cube.colour);
                polygon.setPointCount(4);

                polygon.setPoint(0, sf::Vector2f(coords[indices[0]].x, coords[indices[0]].y));
                polygon.setPoint(1, sf::Vector2f(coords[indices[1]].x, coords[indices[1]].y));
                polygon.setPoint(2, sf::Vector2f(coords[indices[2]].x, coords[indices[2]].y));
                polygon.setPoint(3, sf::Vector2f(coords[indices[3]].x, coords[indices[3]].y));
                
                (*window).draw(polygon);
            };
        };
    };
};