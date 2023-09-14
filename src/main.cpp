#include <SFML/Graphics.hpp>
#include <list>
#include "screen.hpp"
#include "box.hpp"

int main()
{
    // Init
    Screen screen(1000, 1000, "GenEvoSim");
    std::list<Box2D> boxes;

    // Add Boxes
    Box2D box1;
    box1.setSize(sf::Vector2f(100, 50));
    box1.setPosition(50, 100);
    box1.setColor(sf::Color::Blue);
    boxes.push_back(box1);

    // Main loop
    while (screen.window.isOpen()) {
        sf::Event event{};
        while (screen.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                screen.window.close();
            }
        }

        screen.window.clear(sf::Color::White);

        // Draw
        for (const auto& box : boxes) {
            screen.window.draw(box.rectangle);
        }

        screen.window.display();
    }

    return 0;
}