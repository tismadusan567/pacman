#pragma once
#include <SFML/Graphics.hpp>

class Dot
{
public:
    Dot(float x, float y, sf::Texture* texture, bool isBig);
    ~Dot();
    void setActive(bool);
    void Draw(sf::RenderWindow& window);
    bool getActive();
    bool getBig();
    sf::Vector2f getPosition();
    int getPoints();
    sf::Vector2f getSize();
private:
    sf::Texture dotTexture;
    bool isActive;
    bool isBig;
    sf::RectangleShape body;
    int points;
};