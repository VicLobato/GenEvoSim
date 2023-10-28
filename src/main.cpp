#include "graphics/graphics.hpp"
#include "debug/text.hpp"
#include <chrono>
#include <map>

float MOUSE_SENSITIVITY = 0.75;
float MOVEMENT_SPEED = 0.25;
bool INVERT_MOUSE = false;
bool DEBUG = true;

sf::RenderWindow window;
Camera camera = Camera(&window); // Window passed in for rendering
std::map<sf::Keyboard::Key, bool> keys; // Managing key input

// TIMING
std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
std::chrono::time_point<std::chrono::high_resolution_clock> stopTime;
std::chrono::milliseconds duration;

void pre_loop_setup() {
    // Window init
    window.create(sf::VideoMode::getDesktopMode(), "Genetic Evolution Simulator");
    window.setMouseCursorVisible(false);

    // Camera / World init 
    camera.position = {0, 0, 0};
    
    // Mouselock to center screen
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);
};

void end_loop_func() {
    // END LOOP MOUSE LOCK
    int yMouseMultiplier = -1;
    if (INVERT_MOUSE == true) {yMouseMultiplier = 1;}
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get curent mouse screen-space position
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2); // Get screenspace center
    sf::Vector2i deltaMouse = {mousePos.x - windowCenter.x, mousePos.y - windowCenter.y}; // Get change in mouse position
    camera.yRotation += MOUSE_SENSITIVITY * deltaMouse.x / 250.0f;
    camera.xRotation = clamp(camera.xRotation - yMouseMultiplier * MOUSE_SENSITIVITY * deltaMouse.y / 250.0f, -1.5708, 1.5708);
    sf::Mouse::setPosition(windowCenter, window); // Lock mouse to center of screen
};

void event_handler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { // Close window
            window.close();
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect newArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(newArea));
        }
        if (event.type == sf::Event::KeyPressed)  {keys[event.key.code] = true ;}
        if (event.type == sf::Event::KeyReleased) {keys[event.key.code] = false;}

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {window.close();}
    }

    // Forward and Backward Movement
    float sign = 0.0f;
    if (keys[sf::Keyboard::W]) {sign =  1.0f;}
    if (keys[sf::Keyboard::S]) {sign = -1.0f;}
    camera.position.x += std::sin(camera.yRotation) * sign * MOVEMENT_SPEED;
    camera.position.z += std::cos(camera.yRotation) * sign * MOVEMENT_SPEED;

    // Left to Right Movement
    sign = 0.0f;
    if (keys[sf::Keyboard::D]) {sign =  1.0f;}
    if (keys[sf::Keyboard::A]) {sign = -1.0f;}
    camera.position.x += std::sin(camera.yRotation + 1.5708) * sign * MOVEMENT_SPEED;
    camera.position.z += std::cos(camera.yRotation + 1.5708) * sign * MOVEMENT_SPEED;

    // Up and Down movement
    if (keys[sf::Keyboard::Space] ) {camera.position.y += MOVEMENT_SPEED;}
    if (keys[sf::Keyboard::LShift]) {camera.position.y -= MOVEMENT_SPEED;}
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
        camera.render();
        //

        event_handler();

        // POST-EVENT LOGIC
        //

        end_loop_func();
        stopTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
        if (DEBUG == true) { debug_screen(window, camera, duration); } 
        window.display();
    }
};