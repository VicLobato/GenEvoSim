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

    Matrix getRotation() const {
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

        return rotationMatrix;
    }

    std::vector<sf::Vector3f> points() const {
        float w = size.x / 2.0f;
        float h = size.y / 2.0f;
        float d = size.z / 2.0f;
        std::vector<sf::Vector3f> absoluteCoords = {{w,h,d},{w,h,-d},{w,-h,d},{w,-h,-d},{-w,h,d},{-w,h,-d},{-w,-h,d},{-w,-h,-d}};

        for (auto& coord : absoluteCoords) {
            Matrix coordMatrix(3, 1);
            coordMatrix.data = {{coord.x}, {coord.y}, {coord.z}};
            Matrix rotatedCoordMatrix = getRotation() * coordMatrix;
            coord = {
                rotatedCoordMatrix.data[0][0] + position.x,
                rotatedCoordMatrix.data[1][0] + position.y,
                rotatedCoordMatrix.data[2][0] + position.z
            };
        }

        return absoluteCoords;
    }
};