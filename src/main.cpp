#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "graphics.hpp"
#include <iostream> // DEBUGGING
#include <chrono> // Time
#include <thread> // CPU reduction

int main()
{
    // // DO NOT EDIT BELOW THIS LINE
    const int FPS = 60;
    const std::chrono::duration<double> FrameDuration(1.0 / FPS);

    // Physics init
    Solver solver = Solver();
    solver.setUpdateRate(FPS / 30);

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Genetic Evolution Simulator");
    // Draw object
    Draw draw(window, solver);
    draw.DEBUG_MODE = true;

    // // DO NOT EDIT ABOVE THIS LINE

    // Add ground plane and edges
    solver.addObject(Object({0, 10, 0}, 0, Shape({800,10,10}, {100,250,0})));
    solver.addObject(Object({0, 600, 0}, 0, Shape({10,600,10}, {100,250,0})));
    solver.addObject(Object({790, 600, 0}, 0, Shape({10,600,10}, {100,250,0})));

    auto frame_time = std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now();
    // Mainloop
    while (window.isOpen()) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Mainloop logic
        solver.update();
        draw.update(frame_time.count());

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
                    solver.addObject(Object({mPos.x, window.getSize().y - mPos.y, 0}, 1, Shape({10,10,10},{255,255,255})));
                }
            }
            auto frameEnd = std::chrono::high_resolution_clock::now();
            if ((frameEnd - frameStart) > FrameDuration) {break;}
        }
        frame_time = frameEnd - frameStart;
    }
}