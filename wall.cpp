#include <SFML/Graphics.hpp>
#include "wall.h"

Wall::Wall(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture)
{
    body.setSize(size);
    body.setOrigin(size.x/2.0f, size.y/2.0f);
    body.setPosition(position);
    body.setTexture(texture);
}

Wall::~Wall()
{

}

void Wall::Draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void Wall::setAttributes(sf::Vector2f size, sf::Vector2f position, sf::Texture* texture)
{
    body.setSize(size);
    body.setOrigin(size.x/2.0f, size.y/2.0f);
    body.setPosition(position);
    body.setTexture(texture);
}


Collider Wall::getCollider() 
{
    return Collider(body);
}