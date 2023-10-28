#pragma once
#include <SFML/Graphics.hpp>
#include "matrix.hpp"
#include <vector>
#include <cmath>

class Cube {
    public:
        sf::Vector3f position;
        sf::Vector3f size;
        sf::Vector3f rotation;
        sf::Color colour;

    Cube(const sf::Vector3f &_position, const sf::Vector3f &_size, const sf::Vector3f &_rotation = {0, 0, 0}, const sf::Color &_colour = {255, 255, 255})
        : position(_position), size(_size), rotation(_rotation), colour(_colour) {}
};

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

    Camera(sf::RenderWindow *_window) : window(_window) {}

    void render() {
        // Iterate over the cubes and draw them
        for (const auto &cube : objs) {
            // Get width, height and depth but divide by 2
            float w = cube.size.x / 2.0f;
            float h = cube.size.y / 2.0f;
            float d = cube.size.z / 2.0f;

            // Get corner coordinates relative to origin
            std::vector<sf::Vector3f> localCoords{{w,h,d},{w,h,-d},{w,-h,d},{w,-h,-d},{-w,h,d},{-w,h,-d},{-w,-h,d},{-w,-h,-d}};

            // Then offset them by the centre of the cube
            for(auto& coord : localCoords){
                coord.x += cube.position.x;
                coord.y += cube.position.y;
                coord.z += cube.position.z;
            }

            // Precalculate rotation values
            float sinX = std::sin(xRotation);
            float cosX = std::cos(xRotation);
            float sinY = std::sin(yRotation);
            float cosY = std::cos(yRotation);
            float sinZ = std::sin(zRotation);
            float cosZ = std::cos(zRotation);

            Matrix rotationX(3, 3);
            Matrix rotationY(3, 3);
            Matrix rotationZ(3, 3);

            // https://en.wikipedia.org/wiki/3D_projection#Mathematical_formula
            rotationX.data = {{1, 0, 0}, {0, cosX, sinX}, {0, -sinX, cosX}};
            rotationY.data = {{cosY, 0, -sinY}, {0, 1, 0}, {sinY, 0, cosY}};
            rotationZ.data = {{cosZ, sinZ, 0}, {-sinZ, cosZ, 0}, {0, 0, 1}};

            float depth = 1 / std::tan(fov * 3.14159 / 360);

            std::vector<sf::Vector3f> coords; // The corners used for rendering
            for (const auto& localCoord : localCoords) {
                Matrix localCoordMatrix(3, 1);
                localCoordMatrix.data = {
                    {localCoord.x - position.x},
                    {localCoord.y - position.y},
                    {localCoord.z - position.z}
                };
                Matrix rotatedCoord = rotationX * rotationY * rotationZ * localCoordMatrix;
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
                    coord.x += (coord.x + 1) * (*window).getSize().y / 2 + ((*window).getSize().x -  (*window).getSize().y) / 2;
                    coord.y += (coord.y + 1) * (*window).getSize().y / 2;
                }
            } else {
                for (auto &coord : coords) {
                    coord.x += (coord.x + 1) * (*window).getSize().x / 2;
                    coord.y += (coord.y + 1) * (*window).getSize().x / 2 + ((*window).getSize().y -  (*window).getSize().x) / 2;
                }
            }
            
            // Corners now recalculated
            // The 6 faces and the constituent corners
            std::vector<std::vector<int>> polygonIndices = {
                {0, 1, 3},
                {0, 3, 2},
                {4, 5, 7},
                {4, 7, 6},
                {3, 1, 5},
                {3, 5, 7},
                {2, 0, 4},
                {2, 4, 6},
                {5, 1, 0},
                {5, 0, 4},
                {2, 3, 7},
                {2, 7, 6}
            };

            // Render using SFML
            for (const auto &indices : polygonIndices) {
                // https://en.wikipedia.org/wiki/Back-face_culling#Implementation
                int onScreenPoints = 0; // Start by assuming its fully offscreen
                for (int i = 0; i < 3; i++) {
                    if (coords[indices[i]].z >= 0 && coords[indices[i]].z <= CLIP_DISTANCE &&
                        coords[indices[i]].x >= 0 && coords[indices[i]].x <= (*window).getSize().x &&
                        coords[indices[i]].y >= 0 && coords[indices[i]].y <= (*window).getSize().y) {
                        onScreenPoints += 1; // We know its at least partially onscreen
                    }
                }
                if (onScreenPoints == 0) { // If fully offscreen skip
                    continue; 
                } else if (onScreenPoints == 1) { // If 1 point on screen
                    int a = 1;
                }

                sf::ConvexShape polygon;
                polygon.setFillColor(cube.colour);
                polygon.setPointCount(3);

                polygon.setPoint(0, sf::Vector2f(coords[indices[0]].x, coords[indices[0]].y));
                polygon.setPoint(1, sf::Vector2f(coords[indices[1]].x, coords[indices[1]].y));
                polygon.setPoint(2, sf::Vector2f(coords[indices[2]].x, coords[indices[2]].y));
                
                (*window).draw(polygon);
            }
        }
    }   
};