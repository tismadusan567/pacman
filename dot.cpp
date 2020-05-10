#include <SFML/Graphics.hpp>
#include "dot.h"

Dot::Dot(float x, float y, sf::Texture* texture,float size,int points)
{
    isActive = true;
    this->points = points;

    body.setSize(sf::Vector2f(size, size));
    body.setOrigin(body.getSize().x/2.0f, body.getSize().y/2.0f);
    body.setPosition(x, y);
    body.setTexture(texture);
}

Dot::~Dot()
{

}

bool Dot::getActive()
{
    return isActive;
}

void Dot::setActive(bool active)
{
    isActive = active;
}

void Dot::Draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::Vector2f Dot::getPosition()
{
    return body.getPosition();
}

int Dot::getPoints()
{
    return points;
}

sf::Vector2f Dot::getSize()
{
    return body.getSize();
}