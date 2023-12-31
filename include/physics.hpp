#pragma once
#include <SFML/Graphics.hpp> // Vector functionality
#include <iostream> // DEBUGGING

struct Shape {
    sf::Vector3f dimensions;

    Shape() = default;
    Shape(sf::Vector3f dimensions_)
        : dimensions{dimensions_}
    {}
};

struct Object {
    sf::Vector3f pos;
    sf::Vector3f posLast;
    sf::Vector3f acc;
    float mass;
    Shape shape;

    Object() = default;
    Object(sf::Vector3f pos_, float mass_, Shape shape_)
        : pos{pos_}
        , posLast{pos_}
        , acc{0.0f, 0.0f, 0.0f}
        , mass{mass_}
        , shape(shape_)
    {}

    void step(float dt) {
        sf::Vector3f dis = pos - posLast; // Displacement
        posLast = pos;
        pos += dis + acc * (dt * dt);
        acc = {};
    }

    void applyForce(sf::Vector3f force) {
        acc += force / mass; // F = MA -> A = F / M
    }

    void setVelocity(sf::Vector3f vel, float dt) {
        posLast = pos - vel * dt;
    }

    void addVelocity(sf::Vector3f vel, float dt) {
        posLast -= vel * dt;
    }

    sf::Vector3f getVelocity(float dt) {
        return (pos - posLast) / (float)dt;
    }

    void printDebug(float dt) {
        std::cout << "\n";
        sf::Vector3f vel = getVelocity(dt);


        std::cout << "Pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
        std::cout << "Vel: " << vel.x << ", " << vel.y << ", " << vel.z << "\n";
        std::cout << "Acc: " << acc.x << ", " << acc.y << ", " << acc.z << "\n";
    }
};

class Solver {
    public:
        std::vector<Object> objects = {};
        float time = 0;
        float timestep = 1;
        int substeps = 8;
        sf::Vector3f gravity = {0, -1, 0};

    Solver() = default;

    void addObject(Object obj) {
        objects.push_back(obj);
    }

    void removeObject(int index) {
        objects.erase(objects.begin() + index);
    }

    Object& returnObject(int index) {
        return objects[index];
    }

    int getObjectCount() {
        return objects.size();
    }

    void applyGravity() {
        for (Object& obj : objects) {
            obj.applyForce(gravity);
            // obj.printDebug(timestep / substeps);
        }
    }

    void step(float dt) {
        for (Object& obj : objects) {
            obj.step(dt);
        }
    }

    void update() {
        time += timestep;
        const float substep = timestep / substeps;
        for (int counter = 0; counter < substeps; counter++) {
            applyGravity(); // Forces reset after any step
            // resolveCollisions(substep)
            step(substep);
        }
    }

    void setUpdateRate(int rate) {
        timestep = 1 / (double)rate; // Match framerate via reciprocal
    }
};
