#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
public:
    Collider(sf::RectangleShape& body);
    ~Collider();
    void move(float dx, float dy);  

    bool checkCollision(Collider& other, float push);
    sf::Vector2f getPosition();
    sf::Vector2f getHalfSize();

private:
    sf::RectangleShape& body;
};