#pragma once
#include <SFML/Graphics.hpp>

class Dot
{
public:
    Dot(float x, float y, sf::Texture* texture,float size,int points);
    ~Dot();
    void setActive(bool);
    void Draw(sf::RenderWindow& window);
    bool getActive();
    sf::Vector2f getPosition();
    int getPoints();
    sf::Vector2f getSize();
private:
    bool isActive;
    sf::RectangleShape body;
    int points;
};