#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "graphics.hpp"
#include <iostream> // DEBUGGING

int main()
{
    // Physics init
    Solver solver = Solver();

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
    auto obj1 = Object({400,500,0}, 100, Shape({10,10,10}, {200,0,200}));
    obj1.setVelocity({2,2,0},0.2);
    solver.addObject(obj1);

    float frame_time = 0;
    // Mainloop
    while (window.isOpen()) {
        sf::Clock clock;

        // Mainloop logic
        solver.update(frame_time);  // Frame time in seconds as a float
        draw.update(frame_time);

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
        
        auto obj1 = Object({400,500,0}, 100, Shape({10,10,10}, {200,0,200}));
        solver.addObject(obj1);

        sf::Time time = clock.restart();
        frame_time = (static_cast<int>(time.asMicroseconds()))/1000.0;
    }
}