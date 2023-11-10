#pragma once

class Mouse {
    public:
        float sensitivity = 0.75;
        bool invertY = false;
        sf::RenderWindow *window; // Pointer reference to SFML window for reading mouse

        float deltaRotationX = 0;
        float deltaRotationY = 0;
        
    Mouse(sf::RenderWindow *_window) : window(_window) {}

    void read() {
        int yMouseMultiplier = -1;
        if (invertY == true) {yMouseMultiplier = 1;}
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window); // Get curent mouse screen-space position
        sf::Vector2i windowCenter(window->getSize().x / 2, window->getSize().y / 2); // Get screenspace center
        sf::Vector2i deltaMouse = {mousePos.x - windowCenter.x, mousePos.y - windowCenter.y}; // Get change in mouse position
        deltaRotationY = sensitivity * deltaMouse.x / 250.0f;
        deltaRotationX = -yMouseMultiplier * sensitivity * deltaMouse.y / 250.0f ;
        sf::Mouse::setPosition(windowCenter, *window); // Lock mouse to center of screen
    }

    float drX () {return deltaRotationX;}
    float drY () {return deltaRotationY;}
};