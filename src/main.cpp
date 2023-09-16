#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "graphics.hpp"

int main()
{
    // Physics init
    Solver solver = Solver();
    solver.setUpdateRate(30);

    // Object add
    Object obj1 = Object({0, 0, 10}, 1);
    obj1.setVelocity({0, 0, 5}, solver.timestep);
    solver.addObject(obj1);

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode(1920, 1080), "Genetic Evolution Simulator");
    // Draw object
    Draw draw(window);

    // Mainloop
    while (window.isOpen()) {
        sf::Event event; // Handle events

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close window
                window.close();
            }
        }

        draw.update();
    }
}