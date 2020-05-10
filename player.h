#pragma once
#include <SFML/Graphics.hpp>
#include "collider.h"

class Player
{
public:
    Player(float speed, sf::Texture* texture, sf::Texture* texture2);
    ~Player();
    void Update(sf::RenderWindow& window, float deltaTime, float& counter);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Collider getCollider();
private:

    sf::Vector2f movement;
    sf::RectangleShape body;
    float speed;
    bool isClosed;
    sf::Texture *texture, *texture2;
};