#include <SFML/Graphics.hpp>
#include "dot.h"

Dot::Dot(float x, float y, sf::Texture* texture, bool isBig)
{
    isActive = true;
    this->isBig = isBig;
    if(isBig)
    {
        points = 50;
        body.setSize(sf::Vector2f(28.0f, 28.0f));

    }
    else
    {
        points = 10;
        body.setSize(sf::Vector2f(7.0f, 7.0f));
    }
    
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

bool Dot::getBig()
{
    return isBig;
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