#pragma once
#include <map> // Map of all keys

class Keyboard {
    public:
        std::map<sf::Keyboard::Key, bool> keys; 
    
    // 2 birds 1 stone, process event internally, return exit flag
    bool process(sf::Event event) {
        if (event.type == sf::Event::KeyPressed ) {keys[event.key.code] = true ;}
        if (event.type == sf::Event::KeyReleased) {keys[event.key.code] = false;}
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {return true ;}
        return false;
    }

    // Just identify the key
    bool key(const std::string& keyName) {
        // Can't use switch case due to string being non integral type
        if        (keyName == "W") {
            return keys[sf::Keyboard::W];
        } else if (keyName == "A") {
            return keys[sf::Keyboard::A];
        } else if (keyName == "S") {
            return keys[sf::Keyboard::S];
        } else if (keyName == "D") {
            return keys[sf::Keyboard::D];
        } else if (keyName == "Space") {
            return keys[sf::Keyboard::Space];
        } else if (keyName == "LShift") {
            return keys[sf::Keyboard::LShift];
        }
        return false; // Default case
    }
};