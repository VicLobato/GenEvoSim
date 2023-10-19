#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream> // DEBUGGING
#include <string>

void Text(sf::RenderWindow& window, int x, int y, std::string string, sf::Color colour = sf::Color::White, int size = 20, sf::Color highlight = sf::Color::Transparent) {
    // LOAD FONT
    sf::Font font;
    std::string currentWorkingDirectory = std::filesystem::current_path().string();
    std::string fontPath = currentWorkingDirectory + "\\build\\resources\\AnonymousPro-Regular.ttf";
    if (!font.loadFromFile(fontPath)) {
        // VSCODE special because it sets the CWD to the parent folder like a stupid IDE
        std::string fontPath = currentWorkingDirectory + "\\resources\\AnonymousPro-Regular.ttf";
        if (!font.loadFromFile(fontPath)) {
            std::cout << "Failed to load ttf\n";
        }
    }
    
    // DRAW TEXT
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(size);
    text.setFillColor(colour);
    text.setPosition(x, y);

    // BACKGROUND HIGHLIGHT
    sf::FloatRect textBounds = text.getGlobalBounds();
    sf::RectangleShape background(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
    background.setPosition(textBounds.left - 10, textBounds.top - 10);
    background.setFillColor(highlight);

    window.draw(background);
    window.draw(text);
};

class Cube {
    public:
        sf::Vector3f position;
        sf::Vector3f size;

    Cube() = default;
};