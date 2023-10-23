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
        float yRotation = 0; // I gave up, I'm now using euler angles
        float zRotation = 0; // ^ See above
        float fov = 90; // In degrees

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
            rotationX.assign({{1, 0, 0}, {0, cosX, sinX}, {0, -sinX, cosX}});
            rotationY.assign({{cosY, 0, -sinY}, {0, 1, 0}, {sinY, 0, cosY}});
            rotationZ.assign({{cosZ, sinY, 0}, {-sinZ, cosZ, 0}, {0, 0, 1}});

            float depth = 1 / std::tan(fov * 3.14159 / 360);

            std::vector<sf::Vector2f> coords; // The corners used for rendering
            for (const auto& localCoord : localCoords) {
                Matrix localCoordMatrix(3, 1);
                localCoordMatrix.assign({
                    {localCoord.x - position.x},
                    {localCoord.y - position.y},
                    {localCoord.z - position.z}
                });
                Matrix rotatedCoord = rotationX * rotationY * rotationZ * localCoordMatrix;
                coords.push_back({
                    depth * rotatedCoord.data[0][0] / rotatedCoord.data[2][0],
                    depth * rotatedCoord.data[1][0] / rotatedCoord.data[2][0],
                });
            }

            // Map to screen-space coordinates
            // Coords are being returned as -1,-1 to 1,1
            // We need it to go from 0,0 to width,height
            for (auto &coord : coords) {
                coord.x = (coord.x + 1) * (*window).getSize().x / 2;
                coord.y = (coord.y + 1) * (*window).getSize().y / 2;
            }

            for (const auto &coord : coords) {
                sf::CircleShape circle(5);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(coord.x, coord.y); 
                (*window).draw(circle);
            }
            
            // // Corners now recalculated
            // // The 6 faces and the constituent corners
            // std::vector<std::vector<int>> polygonIndices = {
            //     {0, 1, 3, 2},
            //     {4, 5, 7, 6},
            //     {3, 1, 5, 7},
            //     {2, 0, 4, 6},
            //     {5, 1, 0, 4},
            //     {2, 3, 7, 6}
            // };

            // // Render using SFML
            // for (const auto &indices : polygonIndices) {
            //     sf::ConvexShape polygon;
            //     polygon.setFillColor(sf::Color::Blue);
            //     polygon.setPointCount(4);

            //     polygon.setPoint(0, sf::Vector2f(coords[indices[0]].x, coords[indices[0]].y));
            //     polygon.setPoint(1, sf::Vector2f(coords[indices[1]].x, coords[indices[1]].y));
            //     polygon.setPoint(2, sf::Vector2f(coords[indices[2]].x, coords[indices[2]].y));
            //     polygon.setPoint(3, sf::Vector2f(coords[indices[3]].x, coords[indices[3]].y));
            //     (*window).draw(polygon);
            // }

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