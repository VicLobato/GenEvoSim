#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "graphics.hpp"
#include <iostream> // DEBUGGING

int main()
{
    // Physics init
    Solver solver = Solver();
    solver.setUpdateRate(100000); // 30 WHEN FRAME LIMITED

    // Object add
    Object obj1 = Object({400, 500, 0}, 1, Shape({10,10,10}));
    obj1.setVelocity({0, 10, 0}, solver.timestep);
    solver.addObject(obj1);

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Genetic Evolution Simulator");
    // Draw object
    Draw draw(window, solver);

    // Mainloop
    while (window.isOpen()) {
        sf::Event event; // Handle events

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close window
                window.close();
            }
        }

        draw.update();
        solver.update();
    }
}