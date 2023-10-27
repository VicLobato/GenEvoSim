#include <SFML/Graphics.hpp>
#include "graphics.hpp"
#include "matrix.hpp"
#include <iostream> // DEBUGGING
#include <chrono> // TIME

float MOUSE_SENSITIVITY = 0.75;
float MOVEMENT_SPEED = 0.25;
bool INVERT_MOUSE = false;
bool DEBUG = true;

std::string round(auto value, int precision) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

float clamp(float value, float low, float high) { 
    if (value < low) {return low;}
    if (value > high) {return high;}
    else return value;
}

int main()
{
    std::cout << "working\n";   

    // Window init
    sf::RenderWindow window;
    window.create(sf::VideoMode::getDesktopMode(), "Genetic Evolution Simulator");
    window.setMouseCursorVisible(false);

    // Camera / World init 
    Camera camera = Camera(&window); // Window passed in for rendering
    camera.position = {0, 0, 0};
    
    // Mouselock to center screen
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);

    // Pre-loop logic
    // Cube c1 = Cube({0,0,0},{10,0.1,10},{0,0,0},sf::Color(50, 180, 0));
    // camera.objs.push_back(c1);
    Cube c2 = Cube({0,0,5},{1,1,1},{0,0,0},sf::Color(255, 165, 0));
    camera.objs.push_back(c2);

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
                // Forward and Backward Movement
                float sign = 0.0f;
                if (event.key.code == sf::Keyboard::W) {sign = 1.0f;}
                if (event.key.code == sf::Keyboard::S) {sign = -1.0f;}
                camera.position.x += std::sin(camera.yRotation) * MOVEMENT_SPEED * sign;
                camera.position.z += std::cos(camera.yRotation) * MOVEMENT_SPEED * sign;
                // Left and Right movement
                sign = 0.0f;
                if (event.key.code == sf::Keyboard::D) {sign = 1.0f;}
                if (event.key.code == sf::Keyboard::A) {sign = -1.0f;}
                camera.position.x += std::sin(camera.yRotation + 1.5708) * MOVEMENT_SPEED * sign;
                camera.position.z += std::cos(camera.yRotation + 1.5708) * MOVEMENT_SPEED * sign;
                // Up and Down movement
                if (event.key.code == sf::Keyboard::Space) {camera.position.y -= MOVEMENT_SPEED;}
                if (event.key.code == sf::Keyboard::LShift) {camera.position.y += MOVEMENT_SPEED;}
                // Escape
                if (event.key.code == sf::Keyboard::Escape) {window.close();}
            }
        }

        // POST EVENT LOGIC

        // END LOOP MOUSE LOCK
        int yMouseMultiplier = 1;
        if (INVERT_MOUSE == true) {yMouseMultiplier = -1;}
        sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get curent mouse screen-space position
        sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2); // Get screenspace center
        sf::Vector2i deltaMouse = {mousePos.x - windowCenter.x, mousePos.y - windowCenter.y}; // Get change in mouse position
        camera.yRotation += MOUSE_SENSITIVITY * deltaMouse.x / 250.0f;
        camera.xRotation = clamp(camera.xRotation - yMouseMultiplier * MOUSE_SENSITIVITY * deltaMouse.y / 250.0f, -1.5708, 1.5708);
        sf::Mouse::setPosition(windowCenter, window); // Lock mouse to center of screen

        // END LOOP TIMING
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        if (DEBUG == true) {
            std::string debug_string1 = "Frame: " + round(duration.count(), 2) + "ms";
            std::string debug_string2 = "Objs: " + std::to_string(camera.objs.size());
            Text(window, 0, 0, debug_string1, sf::Color::Black, 30, sf::Color::Red);
            Text(window, 0, 40, debug_string2, sf::Color::Black, 30, sf::Color::Green);

            std::string debug_stringpx = "Camera X: " + std::to_string(camera.position.x);
            std::string debug_stringpy = "Camera Y: " + std::to_string(camera.position.y);
            std::string debug_stringpz = "Camera Z: " + std::to_string(camera.position.z);
            Text(window, 0, 0, debug_stringpx, sf::Color::Black, 30, sf::Color::Red, true);
            Text(window, 0, 40, debug_stringpy, sf::Color::Black, 30, sf::Color::Green, true);
            Text(window, 0, 80, debug_stringpz, sf::Color::Black, 30, sf::Color::Blue, true);

            std::string debug_stringcx = "Camera rX: " + std::to_string(camera.xRotation);
            std::string debug_stringcy = "Camera rY: " + std::to_string(camera.yRotation);
            std::string debug_stringcz = "Camera rZ: " + std::to_string(camera.zRotation);
            Text(window, 0, 120, debug_stringcx, sf::Color::Black, 30, sf::Color::Red, true);
            Text(window, 0, 160, debug_stringcy, sf::Color::Black, 30, sf::Color::Green, true);
            Text(window, 0, 200, debug_stringcz, sf::Color::Black, 30, sf::Color::Blue, true);
        } 

        window.display();
    }
}