#include <SFML/Graphics.hpp>
#include "graphics.hpp"
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
    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Genetic Evolution Simulator");

    // Mainloop
    while (window.isOpen()) {
        // START LOOP LOGIC
        auto start = std::chrono::high_resolution_clock::now();
        window.clear();

        // PRE-EVENT LOGIC


        sf::Event event; // Handle events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Close window
                window.close();
            }
        }

        // POST EVENT LOGIC

        // END LOOP LOGIC
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if (DEBUG == true) {
            std::string debug_string = "Frame: " + round(duration.count(), 2) + "ms";
            Text(window, 0, 0, debug_string, sf::Color::Black, 20, sf::Color::Red);
        } 

        window.display();
    }
}