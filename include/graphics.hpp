#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include <iostream> // DEBUGGING

class Draw {
    public:
        sf::RenderWindow& window;
        Solver& solver;

    Draw(sf::RenderWindow& window_, Solver& solver_)
        : window(window_)
        , solver(solver_)
    {}

    void update() {
        window.clear();

        for (Object obj : solver.objects) {
            sf::RectangleShape rectangle;

            rectangle.setSize(sf::Vector2f(obj.shape.dimensions.x, obj.shape.dimensions.y));
            rectangle.setPosition(obj.pos.x, window.getSize().y - obj.pos.y);

            window.draw(rectangle);
        }

        window.display();
    }
};

