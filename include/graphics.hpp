#pragma once
#include <SFML/Graphics.hpp>

class Draw {
    public:
        sf::RenderWindow& window;

    Draw(sf::RenderWindow& renderWindow) : window(renderWindow) { }

    void update() {
        window.clear();

        // Draw shit here
        // window.draw(shape);

        window.display();
    }
};

