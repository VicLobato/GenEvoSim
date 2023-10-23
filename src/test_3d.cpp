#include <SFML/Graphics.hpp>
#include "graphics.hpp"
#include "matrix.hpp"
#include <iostream> // DEBUGGING
#include <chrono> // TIME

// DEBUG MODE
bool DEBUG = true;

std::string round(double value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

int main()
{
    std::cout << "working\n";   

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(), "Genetic Evolution Simulator");

    // Camera / World init
    Camera camera = Camera(&window); // Window passed in for rendering
    camera.position = {0, 0, 5};

    // Pre-loop logic
    Cube c1 = Cube({0,0,0},{1,1,1});
    camera.objs.push_back(c1);

    // Mainloop
    while (window.isOpen()) {
        // START LOOP LOGIC
        auto start = std::chrono::high_resolution_clock::now();
        window.clear();

        // PRE-EVENT LOGIC
        camera.render();

        // EVENT HANDLER
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close window
                window.close();
            }
            if (event.type == sf::Event::Resized) {
                sf::FloatRect newArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(newArea));
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) { camera.position.x += 1; }
                if (event.key.code == sf::Keyboard::D) { camera.position.x -= 1; }
                if (event.key.code == sf::Keyboard::W) { camera.position.y += 1; }
                if (event.key.code == sf::Keyboard::S) { camera.position.y -= 1; }
            }
        }

        // POST EVENT LOGIC

        // END LOOP TIMING
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if (DEBUG == true) {
            std::string debug_string1 = "Frame: " + round(duration.count(), 2) + "ms";
            std::string debug_string2 = "Objs: " + std::to_string(camera.objs.size());
            Text(window, 0, 0, debug_string1, sf::Color::Black, 30, sf::Color::Red);
            Text(window, 0, 40, debug_string2, sf::Color::Black, 30, sf::Color::Green);
        } 

        window.display();
    }
}