#pragma once
#include "matrix.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <filesystem>
#include <iostream> // DEBUGGING
#include <string>

void Text(sf::RenderWindow& window, int x, int y, std::string string, sf::Color colour = sf::Color::White, int size = 20, sf::Color highlight = sf::Color::Transparent) {
    // LOAD FONT
    sf::Font font;
    std::string currentWorkingDirectory = std::filesystem::current_path().string();
    std::string fontPath = currentWorkingDirectory + "\\build\\resources\\AnonymousPro-Regular.ttf";
    if (!font.loadFromFile(fontPath)) {
        // VSCODE special because it sets the CWD to the parent folder like a stupid IDE
        std::string fontPath = currentWorkingDirectory + "\\resources\\AnonymousPro-Regular.ttf";
        if (!font.loadFromFile(fontPath)) {
            std::cout << "Failed to load ttf\n";
        }
    }
    
    // DRAW TEXT
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(size);
    text.setFillColor(colour);
    text.setPosition(x+10, y);

    // BACKGROUND HIGHLIGHT
    sf::FloatRect textBounds = text.getGlobalBounds();
    sf::RectangleShape background(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
    background.setPosition(textBounds.left - 10, textBounds.top - 10);
    background.setFillColor(highlight);

    window.draw(background);
    window.draw(text);
};

class Cube {
    public:
        sf::Vector3f position;
        sf::Vector3f size;
        sf::Vector3f rotation;

    Cube(const sf::Vector3f &_position, const sf::Vector3f &_size, const sf::Vector3f &_rotation = {0, 0, 0})
        : position(_position), size(_size), rotation(_rotation) {}
};

class Camera {
    public:
        sf::Vector3f position = {0, 0, 0};
        float xRotation = 0; // Like a magnetic bearing
        float yRotation = 0; // Relativistic "up" and "down"
        std::vector<Cube> objs; // List of world objects
        sf::RenderWindow *window;

    Camera(sf::RenderWindow *_window) : window(_window) {}

    void render() {
        for (const auto &cube : objs) {
            float w = cube.size.x / 2.0f;
            float h = cube.size.y / 2.0f;
            float d = cube.size.y / 2.0f;

            std::vector<sf::Vector3f> localCoords{
                { w, h, d},
                { w, h,-d},
                { w,-h, d},
                { w,-h,-d},
                {-w, h, d},
                {-w, h,-d},
                {-w,-h, d},
                {-w,-h,-d}
            };

            // Precalculate rotation values
            float sinX = std::sin(cube.rotation.x);
            float cosX = std::cos(cube.rotation.x);
            float sinY = std::sin(cube.rotation.y);
            float cosY = std::cos(cube.rotation.y);
            float sinZ = std::sin(cube.rotation.z);
            float cosZ = std::cos(cube.rotation.z);

            Matrix rotationX(3, 3);
            Matrix rotationY(3, 3);
            Matrix rotationZ(3, 3);

            rotationX.assign({
                {1,    0,     0},
                {0, cosX, -sinX},
                {0, sinX,  cosX}
            });

            rotationY.assign({
                { cosY, 0, sinY},
                {    0, 1,    0},
                {-sinY, 0, cosY}
            });

            rotationZ.assign({
                {cosZ, -sinY, 0},
                {sinZ,  cosZ, 0},
                {   0,     0, 1}
            });

            std::vector<sf::Vector3f> corners; // The corners used for rendering
            for (const auto& localCoord : localCoords) {
                Matrix localCoordMatrix(3, 1);
                localCoordMatrix.assign({
                    {localCoord.x},
                    {localCoord.y},
                    {localCoord.z}
                });
                Matrix rotatedCoord = rotationZ * rotationY * rotationX * localCoordMatrix;
                corners.push_back({
                    cube.position.x + rotatedCoord.data[0][0],
                    cube.position.y + rotatedCoord.data[1][0],
                    cube.position.z + rotatedCoord.data[2][0]
                });
            }

            // Convert to screen-space coordinates
            sf::Vector2u windowSize = (*window).getSize();
            for (auto &corner : corners) {
                float screenX = (corner.x + 1) * windowSize.x * 0.5;
                float screenY = (1 - corner.y) * windowSize.y * 0.5; // Invert the y-axis
                corner = {screenX, screenY, corner.z}; // Update the corner in-place
            }

            // // // // // std::cout << std::to_string(corners[0].x) + " " + std::to_string(corners[1].y) + "\n";

            // Corners now recalculated
            // The 6 faces and the constituent corners
            std::vector<std::vector<int>> polygonIndices = {
                {0, 1, 3, 2},
                {4, 5, 7, 6},
                {3, 1, 5, 7},
                {2, 0, 4, 6},
                {5, 1, 0, 4},
                {2, 3, 7, 6}
            };

            sf::ConvexShape polygon;
            polygon.setFillColor(sf::Color::Blue);
            polygon.setPointCount(4);

            for (const auto &indices : polygonIndices) {
                polygon.setPoint(0, sf::Vector2f(corners[indices[0]].x, corners[indices[0]].y));
                polygon.setPoint(1, sf::Vector2f(corners[indices[1]].x, corners[indices[1]].y));
                polygon.setPoint(2, sf::Vector2f(corners[indices[2]].x, corners[indices[2]].y));
                polygon.setPoint(3, sf::Vector2f(corners[indices[3]].x, corners[indices[3]].y));
                (*window).draw(polygon);
            }

        }
    }

    sf::Vector2f project(sf::Vector3f obj) {
        // Transformation matrix, sets the camera to the origin
        Matrix transform(4, 4);
        transform.assign({
            {1, 0, 0, -position.x},
            {0, 1, 0, -position.y},
            {0, 0, 1, -position.z},
            {0, 0, 0, 1          }
        });

        // Perspective matrix, applies a rotation relative to the camera
        Matrix perspective(4, 4);
        perspective.assign({
            {1, 0,                   0, 0},
            {0, std::tan(xRotation), 0, 0},
            {0, 0, std::tan(yRotation), 0},
            {0, 0,                   0, 1}
        });

        // Point matrix, holds the 3d point as a 4d matrix
        Matrix point(4, 1);
        point.assign({
            {obj.x},
            {obj.y},
            {obj.z},
            {1    }
        });

        // Output matrix, gets x' y' z' w'
        Matrix output(4, 1);
        output = transform * perspective * point;
        
        float xProj = output.data[0][0] / output.data[3][0]; // X = x' / w'
        float yProj = output.data[1][0] / output.data[3][0]; // Y = y' / w'

        sf::Vector2f out = {xProj, yProj};
        return out;
    }
};