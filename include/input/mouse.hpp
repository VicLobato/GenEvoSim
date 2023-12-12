#pragma once

class Mouse {
    public:
        float sensitivity = 10; // Constant multiplier
        bool invertY = false; // Useful setting
        sf::RenderWindow *window; // Pointer reference to SFML window for reading mouse

        // Tracks how many pixels the mouse moves from the centre of the window each frame
        float drX = 0;
        float drY = 0;
        
    Mouse(sf::RenderWindow *_window) : window(_window) {} // Constructor

    void read() {
        // Due to inverse screen-space mapping, where the top is 0 and the bottom is 1
        // We default our y multiplier to -1, so that the mouse moving up is positive
        // And invert if the invertY flag is set high
        int yMouseMultiplier = -1;
        if (invertY == true) {yMouseMultiplier = 1;}

        // Get curent mouse screen-space position as an integer vector2
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        // Get screenspace center
        sf::Vector2i windowCenter(window->getSize().x / 2, window->getSize().y / 2);

        // Get change in mouse position from center
        sf::Vector2i deltaMouse = {mousePos.x - windowCenter.x, mousePos.y - windowCenter.y};
        // Map to deltaX rotation and deltaY rotation (drX / drY)
        drY = sensitivity * deltaMouse.x / 250.0f;
        drX = yMouseMultiplier * sensitivity * deltaMouse.y / 250.0f;

        // Set mouse position back to center
        // To avoid mouse going off-screen in windowed mode
        sf::Mouse::setPosition(windowCenter, *window);
    }
};