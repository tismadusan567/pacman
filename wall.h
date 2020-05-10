#pragma once
#include <SFML/Graphics.hpp>
#include "collider.h"

class Wall
{
public:
    Wall(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture);
    ~Wall();
    void Draw(sf::RenderWindow& window);
    void setAttributes(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture);
    Collider getCollider();
private:
    sf::RectangleShape body;
};