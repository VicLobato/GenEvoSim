#include <SFML/Graphics.hpp>
#include "Screen.hpp"

int main()
{
    // Init
    Screen screen(1000, 1000, "GenEvoSim");

    // Main loop
    while (screen.window.isOpen()) {
        sf::Event event{};
        while (screen.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                screen.window.close();
            }
        }

        screen.window.clear(sf::Color::White);
        screen.window.display();
    }

    return 0;
}