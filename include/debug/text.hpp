#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include "debug/process.hpp"

void draw_text(sf::RenderWindow& window, int x, int y, std::string string, sf::Color colour = sf::Color::White, int size = 20, sf::Color highlight = sf::Color::Transparent, bool anchorRight = false, bool anchorBottom = false) {
    // LOAD FONT
    sf::Font font;
    std::string currentWorkingDirectory = std::filesystem::current_path().string();
    std::string fontPath = currentWorkingDirectory + "\\resources\\AnonymousPro-Regular.ttf";
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
    background.setFillColor(highlight);
    float nX = x;
    float nY = y;

    if (anchorRight  == true) {nX = window.getSize().x - x - textBounds.width  - 20;}
    if (anchorBottom == true) {nY = window.getSize().y - y - textBounds.height - 20;}
    

    if (anchorRight == true || anchorBottom == true) {
        text.setPosition(nX,  nY);
        textBounds = text.getGlobalBounds();
    }

    background.setPosition(textBounds.left - 10, textBounds.top - 10);

    window.draw(background);
    window.draw(text);
};

void debug_screen(sf::RenderWindow& window, Camera camera, std::chrono::milliseconds duration) {
    std::string debug_string1 = "Frame: " + round(duration.count(), 2) + "ms";
    std::string debug_string2 = "Objs: " + std::to_string(camera.objs.size());
    draw_text(window, 0, 0, debug_string1, sf::Color::Black, 30, sf::Color::Red);
    draw_text(window, 0, 40, debug_string2, sf::Color::Black, 30, sf::Color::Green);

    std::string debug_stringpx = "Camera X: " + std::to_string(camera.position.x);
    std::string debug_stringpy = "Camera Y: " + std::to_string(camera.position.y);
    std::string debug_stringpz = "Camera Z: " + std::to_string(camera.position.z);
    draw_text(window, 0, 0, debug_stringpx, sf::Color::Black, 30, sf::Color::Red, true);
    draw_text(window, 0, 40, debug_stringpy, sf::Color::Black, 30, sf::Color::Green, true);
    draw_text(window, 0, 80, debug_stringpz, sf::Color::Black, 30, sf::Color::Blue, true);

    std::string debug_stringcx = "Camera rX: " + std::to_string(camera.xRotation);
    std::string debug_stringcy = "Camera rY: " + std::to_string(camera.yRotation);
    std::string debug_stringcz = "Camera rZ: " + std::to_string(camera.zRotation);
    draw_text(window, 0, 120, debug_stringcx, sf::Color::Black, 30, sf::Color::Red, true);
    draw_text(window, 0, 160, debug_stringcy, sf::Color::Black, 30, sf::Color::Green, true);
    draw_text(window, 0, 200, debug_stringcz, sf::Color::Black, 30, sf::Color::Blue, true);

    std::string debug_stringh = "WASD for Movement\nSpace and Shift for Up and Down\nESC to exit";
    draw_text(window, 0, 90, debug_stringh, sf::Color::White, 30, sf::Color::Transparent, false, true);
};