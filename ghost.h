#pragma once
#include <SFML/Graphics.hpp>
#include "collider.h"

class Ghost
{
public:
    Ghost(const float& speed,const std::string& color, bool boolGrid[28][36]);
    ~Ghost();
    void Update(sf::RenderWindow& window, const float& deltaTime, float& counter, const sf::Vector2f& pacmanPosition, const sf::Vector2f& pacmanMovement, const sf::Vector2f& redGhostPosition);
    void Draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Collider getCollider();
    bool getScared();
    void setScared(const bool& isScared);
    void setSpeed(const float& speed);
    void adjustTarget(sf::Vector2i &target, const sf::Vector2f& pacmanPosition, const sf::Vector2f& pacmanMovement, const sf::Vector2f& redGhostPosition);
    std::vector<char> validPaths(const int& x, const int& y);
    sf::Vector2i pathfind(const sf::Vector2i& start, const sf::Vector2i& target);
    void die();
private:
    bool boolGrid[29][37];
    sf::Vector2f movement;
    sf::RectangleShape body;
    float speed;
    int animationNum;
    sf::Texture texture, texture2;
    sf::Vector2i nextTile;
    std::string color;
    bool isScared;
};