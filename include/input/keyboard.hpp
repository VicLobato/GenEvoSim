#pragma once
#include <iostream>
#include <map> // Map of all keys

// Just useful
float rad(float degrees) {
    return degrees * 3.14159 / 180.0f;
}

class Keyboard {
    public:
        // Essentially a map of whether a key is pressed or not
        // The easiest way to track keys being held down
        std::map<sf::Keyboard::Key, bool> keys;
        float movement_speed = 0.25; // Constant movement speed
        Camera *camera; // Camera pointer (movement is handled inside this hpp)
    
    Keyboard(Camera *_camera) : camera(_camera) {} // Constructor

    // Useful for when testing in main - easy way to check if a key is pressed
    bool key(sf::Keyboard::Key code) {
        return keys[code];
    }
    
    // 2 birds 1 stone, process event internally, return exit flag
    // It is a boolean so we can exit the window in the mainloop
    bool process(sf::Event event) {
        if (event.type == sf::Event::KeyPressed ) {keys[event.key.code] = true ;}
        if (event.type == sf::Event::KeyReleased) {keys[event.key.code] = false;}
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {return true ;}
        // Return true if escape is pressed ^^^

        // Forward and Backward Movement
        float sign = 0.0f;
        if (keys[sf::Keyboard::W]) {sign =  1.0f;}
        if (keys[sf::Keyboard::S]) {sign = -1.0f;}
        camera->position.x += std::sin(rad(camera->rotation.y)) * sign * movement_speed;
        camera->position.z -= std::cos(rad(camera->rotation.y)) * sign * movement_speed;

        // Left to Right Movement
        sign = 0.0f;
        if (keys[sf::Keyboard::D]) {sign =  1.0f;}
        if (keys[sf::Keyboard::A]) {sign = -1.0f;}
        camera->position.x += std::cos(rad(camera->rotation.y)) * sign * movement_speed; // Sin + 90 = Cos
        camera->position.z += std::sin(rad(camera->rotation.y)) * sign * movement_speed; // Cos + 90 = - Sin

        // Up and Down movement
        if (keys[sf::Keyboard::Space]) {camera->position.y += movement_speed;}
        if (keys[sf::Keyboard::LShift]) {camera->position.y -= movement_speed;}

        // Default return false
        return false;
    }
};