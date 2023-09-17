#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include <iostream> // DEBUGGING
#include <string> // DEBUGGING

class Draw {
    public:
        sf::RenderWindow& window;
        Solver& solver;
        bool DEBUG_MODE;

    Draw(sf::RenderWindow& window_, Solver& solver_)
        : window(window_)
        , solver(solver_)
    {}

    void update(float frame_time) {
        window.clear();

        for (Object obj : solver.objects) {
            sf::RectangleShape rectangle;

            rectangle.setFillColor(obj.shape.colour);
            rectangle.setSize(sf::Vector2f(obj.shape.dimensions.x, obj.shape.dimensions.y));
            rectangle.setPosition(obj.pos.x, window.getSize().y - obj.pos.y);

            window.draw(rectangle);
        }
        if (DEBUG_MODE == true) {
            std::string debug_string = "Frame Time: " + std::to_string(frame_time) + "\nObjects: " + std::to_string(solver.objects.size());

            sf::Text text;
            sf::Font font;
            font.loadFromFile("AnonymousPro-Regular.ttf");
            text.setFont(font);
            text.setString(debug_string);
            text.setCharacterSize(40);
            text.setFillColor(sf::Color::White);
            text.setPosition(0, 0);
        }

        window.display();
    }
};

