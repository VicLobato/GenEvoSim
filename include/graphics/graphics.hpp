#pragma once
#include "matrix.hpp"
#include "cube.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
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
        sf::Vector3f rotation = {0, 0, 0}; // Euler angles

        sf::Color skyColour = {74, 143, 168};

        std::vector<Cube> objs; // List of world objects
        sf::RenderWindow *window; // Pointer reference to SFML window for rendering

    Camera(sf::RenderWindow *_window) : window(_window) {}

    void setPerspectiveProjection(float fov, float nearPlane, float farPlane) {
        glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LEQUAL);
        // glDepthMask(GL_TRUE);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float aspectRatio = static_cast<float>(window->getSize().x) / window->getSize().y;
        float f = 1.0f / tan(fov * 0.5f);
        float rangeInv = 1.0f / (nearPlane - farPlane);

        glFrustum(-aspectRatio * nearPlane * f, aspectRatio * nearPlane * f, -nearPlane * f, nearPlane * f, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

    // Essentially adjust colour between 100% and 50% depending on angle relative to surface
    sf::Color globalIllumination(sf::Color rawColour, sf::Vector3f p1, sf::Vector3f p2, sf::Vector3f p3) {
        // Lower and upper illumination values
        float lumLow = 0.5;
        float lumHigh = 1;

        sf::Vector3f v1 = p2 - p1;
        sf::Vector3f v2 = p3 - p1;

        sf::Vector3f normal = sf::Vector3f(
            (v1.y * v2.z) - (v1.z * v2.y),
            (v1.z * v2.x) - (v1.x * v2.z),
            (v1.x * v2.y) - (v1.y * v2.x)
        );

        float cos = 0.5 + (normal.y / distance(normal, {0, 0, 0})) / 2;
        float scalar = lumLow + cos * (lumHigh - lumLow);// DO CALCULATIONS

        sf::Color colourOut = {
            static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(round(rawColour.r * scalar))))),
            static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(round(rawColour.g * scalar))))),
            static_cast<sf::Uint8>(std::min(255, std::max(0, static_cast<int>(round(rawColour.b * scalar)))))
        };

        return colourOut;
    }

    void render() {
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        
        glRotatef(-rotation.x,1.0,0.0,0.0);
        glRotatef( rotation.y,0.0,1.0,0.0);
        glRotatef( rotation.z,0.0,0.0,1.0);
        glTranslatef(-position.x, -position.y, -position.z);

        // Iterate over the cubes and draw them
        for (const auto &cube : objs) {
            std::vector<sf::Vector3f> coords = cube.points(); // The corners used for screen-space
            std::vector<int> indices = {
                2,3,1,0,
                4,5,7,6,
                0,1,5,4,
                3,2,6,7,
                4,6,2,0,
                1,3,7,5
            };

            glPushMatrix();
            for (int i=0; i<24; i+=4) {
                sf::Vector3f p1 = coords[indices[i  ]];
                sf::Vector3f p2 = coords[indices[i+1]];
                sf::Vector3f p3 = coords[indices[i+2]];
                sf::Vector3f p4 = coords[indices[i+3]];

                sf::Color adjustedColour = globalIllumination(cube.colour, p1, p2, p3);

                glBegin(GL_QUADS);
                glColor3ub(adjustedColour.r, adjustedColour.g, adjustedColour.b);
                glVertex3f(p1.x, p1.y, p1.z);
                glVertex3f(p2.x, p2.y, p2.z);
                glVertex3f(p3.x, p3.y, p3.z);
                glVertex3f(p4.x, p4.y, p4.z);
                glEnd();
                glPopMatrix();
            }
        }
    }
};