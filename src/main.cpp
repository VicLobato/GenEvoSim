#include "engine/graphics.hpp"
#include "engine/physics.hpp"
#include "debug/text.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

bool DEBUG = true;

// WINDOW
sf::RenderWindow window;
Camera camera = Camera(&window); // Window passed in for rendering

// INPUT
Keyboard keyboard = Keyboard(&camera);
Mouse mouse = Mouse(&window);

// TIMING
sf::Clock frameClock;
sf::Time elapsed;
int duration = 1;

void pre_loop_setup() {
    // Window init
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.antialiasingLevel = 4;

    window.create(sf::VideoMode::getDesktopMode(), "Genetic Evolution Simulator", sf::Style::Default, settings);
    window.setMouseCursorVisible(false);
    window.setActive(true);
    camera.setPerspectiveProjection(90, 0.1, 100);

    // Camera / World init 
    camera.position = {5, 5, 0};
    camera.rotation.x = -45;
    camera.rotation.y = 270;
    
    // Mouselock to center screen
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);
};

void end_loop_func() {
    // END LOOP MOUSE LOCK
    mouse.read();
    camera.rotation.y = fmod(camera.rotation.y + mouse.drY + 360, 360.0);
    camera.rotation.x = clamp(camera.rotation.x + mouse.drX, -90, 90);
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
    Cube ground = Cube({0,-1,0},{100,1,100},{0,0,0},sf::Color(125, 117, 107));
    camera.objs.push_back(ground);

    //Cube c1 = Cube({1,0,0},{1,1,1},{0,0,0},sf::Color(255, 50, 0));
    //camera.objs.push_back(c1);
    Cube c2 = Cube({0,3,0},{1,1,1},{0,0,0},sf::Color(0, 255, 50));
    camera.objs.push_back(c2);
    Cube c3 = Cube({0,1.5,0},{1,1,1},{0,0,0},sf::Color(50, 0, 255));
    camera.objs.push_back(c3);


    // Mainloop
    while (window.isOpen()) {
        frameClock.restart();
        window.clear(camera.skyColour);

        // PRE-EVENT LOGIC
        if (keyboard.key(sf::Keyboard::Num1)) {camera.objs[2].position.y += 0.1;}
        if (keyboard.key(sf::Keyboard::Num2)) {camera.objs[2].position.y -= 0.1;}
        //

        collisionDetection(camera.objs);
        camera.render();
        event_handler();

        // POST-EVENT LOGIC
        //

        end_loop_func();
        window.pushGLStates();
        if (DEBUG == true) {debug_screen(window, camera, duration);} 
        window.popGLStates(); // Test window.resetGLStates()
        window.display();

        elapsed = frameClock.getElapsedTime();
        duration = static_cast<int>(elapsed.asMilliseconds());
    }
};