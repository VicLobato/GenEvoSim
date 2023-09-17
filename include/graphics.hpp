#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include <filesystem>
#include <iostream> // DEBUGGING
#include <string> // DEBUGGING SCREEN

class Draw {
    public:
        sf::RenderWindow& window;
        sf::Font font;
        Solver& solver;
        bool DEBUG_MODE;

    Draw(sf::RenderWindow& window_, Solver& solver_)
        : window(window_)
        , solver(solver_)
    {   
        std::string currentWorkingDirectory = std::filesystem::current_path().string();
        std::string fontPath = currentWorkingDirectory + "\\build\\AnonymousPro-Regular.ttf";
        if (!font.loadFromFile(fontPath)) {
            // VSCODE special because it sets the CWD to the parent folder like a stupid IDE
            std::string fontPath = currentWorkingDirectory + "\\AnonymousPro-Regular.ttf";
            if (!font.loadFromFile(fontPath)) {
                std::cout << "Failed to load ttf\n";
            }
        }
    }

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
            std::string debug_string = "Time: " + std::to_string(solver.time / 1000) + "s\nFrame: " + std::to_string(frame_time) + "ms\nObjects: " + std::to_string(solver.objects.size());

            sf::Text text;
            text.setFont(font);
            text.setString(debug_string);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::White);
            text.setPosition(0, 0);
            window.draw(text);
        }

        window.display();
    }
};

