#pragma once
#include "graphics/graphics.hpp"
#include <map> // Map of all keys

class Keyboard {
    public:
        std::map<sf::Keyboard::Key, bool> keys;
        float movement_speed = 0.25;
        Camera *camera;
    
    Keyboard(Camera *_camera) : camera(_camera) {}
    
    // 2 birds 1 stone, process event internally, return exit flag
    bool process(sf::Event event) {
        if (event.type == sf::Event::KeyPressed ) {keys[event.key.code] = true ;}
        if (event.type == sf::Event::KeyReleased) {keys[event.key.code] = false;}
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {return true ;}

        // Forward and Backward Movement
        float sign = 0.0f;
        if (keys[sf::Keyboard::W]) {sign =  1.0f;}
        if (keys[sf::Keyboard::S]) {sign = -1.0f;}
        camera->position.x += std::sin(camera->yRotation) * sign * movement_speed;
        camera->position.z += std::cos(camera->yRotation) * sign * movement_speed;

        // Left to Right Movement
        sign = 0.0f;
        if (keys[sf::Keyboard::D]) {sign =  1.0f;}
        if (keys[sf::Keyboard::A]) {sign = -1.0f;}
        camera->position.x += std::sin(camera->yRotation + 1.5708) * sign * movement_speed;
        camera->position.z += std::cos(camera->yRotation + 1.5708) * sign * movement_speed;

        // Up and Down movement
        if (keys[sf::Keyboard::Space]) {camera->position.y += movement_speed;}
        if (keys[sf::Keyboard::LShift]) {camera->position.y -= movement_speed;}

        return false;
    }
};