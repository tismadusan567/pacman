#pragma once
#include <SFML/Graphics.hpp>
#include "collider.h"

class Ghost
{
public:
    Ghost(float speed, sf::Vector2f position, sf::Texture* texture, sf::Texture* texture2);
    ~Ghost();
    void Update(sf::RenderWindow& window, float deltaTime, float& counter);
    void Draw(sf::RenderWindow& window);
    void setDirection(int randDirection);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Collider getCollider();
private:
    int randDirection;
    sf::Vector2f movement;
    sf::RectangleShape body;
    float speed;
    int animationNum;
    sf::Texture *texture, *texture2;
};