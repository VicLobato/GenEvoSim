#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "graphics.hpp"
#include <iostream> // DEBUGGING
#include <chrono> // Time
#include <thread> // CPU reduction

int main()
{
    const int FPS = 30;
    const std::chrono::duration<double> FrameDuration(1.0 / FPS);

    // Physics init
    Solver solver = Solver();
    solver.setUpdateRate(1); // 30 WHEN FRAME LIMITED

    // Object add
    Object obj1 = Object({400, 500, 0}, 1, Shape({10,10,10}));
    obj1.setVelocity({1, 1, 0}, solver.timestep);
    solver.addObject(obj1);

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Genetic Evolution Simulator");
    // Draw object
    Draw draw(window, solver);

    // Mainloop
    while (window.isOpen()) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Mainloop logic
        draw.update();
        solver.update();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        // FPS limiter
        while (true) {
            sf::Event event; // Handle events
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { // Close window
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mPos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    solver.addObject(Object({mPos.x, window.getSize().y - mPos.y, 0}, 1, Shape({10,10,10})));
                }
            }
            auto frameEnd = std::chrono::high_resolution_clock::now();
            if ((frameEnd - frameStart) > FrameDuration) {break;}
        }
    }
}