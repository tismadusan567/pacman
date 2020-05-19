#pragma once
#include <SFML/Graphics.hpp>
#include "collider.h"

class Ghost
{
public:
    Ghost(float speed, sf::Vector2f position,const std::string& color, bool boolGrid[28][36]);
    ~Ghost();
    void Update(sf::RenderWindow& window, float deltaTime, float& counter, sf::Vector2f pacmanPosition);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Collider getCollider();
    std::vector<sf::Vector2i> bfs(sf::Vector2i start, sf::Vector2i target);
private:
    bool boolGrid[28][36];
    sf::Vector2f movement;
    sf::RectangleShape body;
    float speed;
    int animationNum;
    sf::Texture texture, texture2;
    sf::Vector2i nextTile;
};