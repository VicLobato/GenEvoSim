#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <string>
#include "debug/process.hpp"

void draw_text(sf::RenderWindow& window, int x, int y, std::string string, sf::Color colour = sf::Color::White, int size = 20, sf::Color highlight = sf::Color::Transparent, bool anchorRight = false, bool anchorBottom = false) {
    // // LOAD FONT
    sf::Font font;
    // Fonts are annoying, they only take absolute paths
    // Take CurrentWorkingDirectory (the file path of where the executable is)
    std::string currentWorkingDirectory = std::filesystem::current_path().string();
    // And add the relative path to the font if (will always be in the folder resources)
    std::string fontPath = currentWorkingDirectory + "\\resources\\AnonymousPro-Regular.ttf";

    // If the font fails to load, print an error
    if (!font.loadFromFile(fontPath)) {
        std::cout << "Failed to load ttf\n";
    }
    
    // // DRAW TEXT
    // Configure according to parameters
    sf::Text text;
    text.setFont(font);
    text.setString(string);
    text.setCharacterSize(size);
    text.setFillColor(colour);
    text.setPosition(x, y);

    // BACKGROUND HIGHLIGHT
    sf::FloatRect textBounds = text.getGlobalBounds();
    // Offset by 20 to add slight visual padding, this only changed shape
    sf::RectangleShape background(sf::Vector2f(textBounds.width + 20, textBounds.height + 20));
    // Set colour to highlight
    background.setFillColor(highlight);
    // "newX and newY"
    float nX = x;
    float nY = y;

    // If we anchor to the right or bottom, update the newX and newY
    // Default anchor is top left
    // Which means positive values move it from the top left, to the right and down
    // Anchoring useful for when you want to keep it exactly to the right or bottom
    // Without having to account for varying window sizes every time
    if (anchorRight  == true) {nX = window.getSize().x - x - textBounds.width  - 20;}
    if (anchorBottom == true) {nY = window.getSize().y - y - textBounds.height - 20;}
    
    // If we're anchoring anywhere other than top right
    if (anchorRight == true || anchorBottom == true) {
        // We move the text to the new position
        text.setPosition(nX,  nY);
        // And update its bounds for the highlight
        textBounds = text.getGlobalBounds();
    }
    // Highlight position set - works regardless of whether text was moved or not
    // Because text is in correct position, background will move to it
    background.setPosition(textBounds.left - 10, textBounds.top - 10);

    // Render background first, so text appears over it
    window.draw(background);
    window.draw(text);
};

// This large hellish statement basically just prints a bunch of debug values
// Doesn't matter that it's annoying, it works and won't be used in releases
void debug_screen(sf::RenderWindow& window, Camera camera, int duration) {
    std::string debug_string1 = "Frame: " + round(duration, 2) + "ms";
    std::string debug_string2 = "Objs: " + std::to_string(camera.objs.size());
    std::string debug_string3 = "FPS: " + std::to_string(1000 / (duration + 1));
    draw_text(window, 0, 0, debug_string1, sf::Color::Black, 30, sf::Color::Red);
    draw_text(window, 0, 40, debug_string2, sf::Color::Black, 30, sf::Color::Green);
    draw_text(window, 0, 80, debug_string3, sf::Color::Black, 30, sf::Color::Blue);

    std::string debug_stringpx = "Camera X: " + std::to_string(camera.position.x);
    std::string debug_stringpy = "Camera Y: " + std::to_string(camera.position.y);
    std::string debug_stringpz = "Camera Z: " + std::to_string(camera.position.z);
    draw_text(window, 0, 0, debug_stringpx, sf::Color::Black, 30, sf::Color::Red, true);
    draw_text(window, 0, 40, debug_stringpy, sf::Color::Black, 30, sf::Color::Green, true);
    draw_text(window, 0, 80, debug_stringpz, sf::Color::Black, 30, sf::Color::Blue, true);

    std::string debug_stringcx = "Camera rX: " + std::to_string(camera.rotation.x);
    std::string debug_stringcy = "Camera rY: " + std::to_string(camera.rotation.y);
    draw_text(window, 0, 120, debug_stringcx, sf::Color::Black, 30, sf::Color::Red, true);
    draw_text(window, 0, 160, debug_stringcy, sf::Color::Black, 30, sf::Color::Green, true);

    std::string debug_stringh = "WASD for Movement\nSpace and Shift for Up and Down\nESC to exit";
    draw_text(window, 0, 90, debug_stringh, sf::Color::White, 30, sf::Color::Transparent, false, true);
};