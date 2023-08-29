#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>

class Screen {
public:
    bool DEBUG_MODE = false;
    sf::RenderWindow window;

    Screen() = default;
    Screen(int width, int height, const char* title) {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 1;
        window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);
        window.setFramerateLimit(60);
    }
};