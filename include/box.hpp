#pragma once
#include <SFML/Graphics.hpp>

struct Box2D {
	sf::RectangleShape rectangle;
    sf::Color color;

    void setSize(sf::Vector2f size) { rectangle.setSize(size); }
    void setPosition(int x, int y) { rectangle.setPosition(x, y); }
    void setX(int x) { rectangle.setPosition(x, rectangle.getPosition().y); }
    void setY(int y) { rectangle.setPosition(rectangle.getPosition().x, y); }

    sf::Vector2f getsize() { return rectangle.getSize(); }
    sf::Vector2f getPosition() { return rectangle.getPosition(); }

    void setColor(sf::Color color) { rectangle.setFillColor(color); }  
};