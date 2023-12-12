#pragma once
#include <SFML/Graphics.hpp>
#include "maths/matrix.hpp"
#include <iostream>
#include <vector>
#include <cmath>    

// Class for storing cube data
// Cube name used as Cubeoid annoying to type
class Cube {
    public:
        // Vectors used to standarise data
        sf::Vector3f position; // Centre of cube
        sf::Vector3f size;     // Edge lengths
        sf::Vector3f rotation; // Rotation around x, y, z axes
        sf::Color colour;      // Colour

        // Mutable coords, updated upon function call + change
        mutable std::vector<sf::Vector3f> coords;

        // Used for updating absolute coords faster
        // Mutable to make easier
        mutable sf::Vector3f oldSize = {0, 0, 0}; 
        mutable sf::Vector3f oldRotation = {0, 0, 0};
        mutable sf::Vector3f oldPosition = {0, 0, 0};

    // Constructor
    Cube(const sf::Vector3f &_position, const sf::Vector3f &_size, const sf::Vector3f &_rotation = {0, 0, 0}, const sf::Color &_colour = {255, 255, 255})
        : position(_position), size(_size), rotation(_rotation), colour(_colour) {}


    // Function to return coordinates as absolute positions
    std::vector<sf::Vector3f> points() const {
        // Check if nothing has changed
        if (oldRotation != rotation || oldPosition != position || oldSize != size) {
            // Update old variables
            oldPosition = position;
            oldRotation = rotation;
            oldSize = size;

            // Half side lengths, from centre to edge
            float w = size.x / 2.0f;
            float h = size.y / 2.0f;
            float d = size.z / 2.0f;

            // These are the relative coordinates to the centre, but we store in absolute
            // As we will update and return them
            std::vector<sf::Vector3f> absoluteCoords = {{w,h,d},{w,h,-d},{w,-h,d},{w,-h,-d},{-w,h,d},{-w,h,-d},{-w,-h,d},{-w,-h,-d}};

            // Create the rotation matrix
            float cosX = std::cos(rotation.x);
            float sinX = std::sin(rotation.x);
            float cosY = std::cos(rotation.y);
            float sinY = std::sin(rotation.y);
            float cosZ = std::cos(rotation.z);
            float sinZ = std::sin(rotation.z);

            Matrix rotationMatrix;
            rotationMatrix.data = {
                {cosY * cosZ, -cosY * sinZ, sinY},
                {cosX * sinZ + sinX * cosZ * sinY, cosX * cosZ - sinX * sinZ * sinY, -sinX * cosY},
                {sinX * sinZ - cosX * cosZ * sinY, cosZ * sinX + cosX * sinZ * sinY, cosX * cosY}
            };

            // Iterate over each coordinate and recalculate it
            for (auto& coord : absoluteCoords) {
                Matrix coordMatrix(3, 1);
                coordMatrix.data = {{coord.x}, {coord.y}, {coord.z}};
                Matrix rotatedCoordMatrix = rotationMatrix * coordMatrix;
                coord = {
                    rotatedCoordMatrix.data[0][0] + position.x,
                    rotatedCoordMatrix.data[1][0] + position.y,
                    rotatedCoordMatrix.data[2][0] + position.z
                };
            }

            coords = absoluteCoords;
        }
        
        // Return list
        return coords;
    }
};