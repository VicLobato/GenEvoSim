#include "graphics/graphics.hpp"
#include "debug/text.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include <chrono>

bool DEBUG = true;

// WINDOW
sf::RenderWindow window;
Camera camera = Camera(&window); // Window passed in for rendering

// INPUT
Keyboard keyboard = Keyboard(&camera);
Mouse mouse = Mouse(&window);

// TIMING
std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
std::chrono::microseconds duration;

void pre_loop_setup() {
    // Window init
    window.create(sf::VideoMode::getDesktopMode(), "Genetic Evolution Simulator");
    window.setMouseCursorVisible(false);

    // Camera / World init 
    // camera.position = {3, 4, 0};
    // camera.yRotation = -0.441939221;
    
    // Mouselock to center screen
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);
};

void end_loop_func() {
    // END LOOP MOUSE LOCK
    mouse.read();
    camera.yRotation += mouse.drY;
    camera.xRotation = clamp(camera.xRotation + mouse.drX, -1.5708, 1.5708);
};

void event_handler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Resized) {
            sf::FloatRect newArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(newArea));
        }

        if (keyboard.process(event)) {window.close();}
        if (event.type == sf::Event::Closed) {window.close();}
    }
};

int main() {
    std::cout << "working\n";

    pre_loop_setup();

    // Pre-loop logic
    Cube c2 = Cube({0,0,5},{1,1,1},{0,0,0},sf::Color(255, 165, 0));
    camera.objs.push_back(c2);

    // Mainloop
    while (window.isOpen()) {
        startTime = std::chrono::high_resolution_clock::now();
        window.clear();

        // PRE-EVENT LOGIC
        //

        camera.render();
        event_handler();

        // POST-EVENT LOGIC
        //

        end_loop_func();
        stopTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
        if (DEBUG == true) { debug_screen(window, camera, duration); } 
        window.display();
    }
};