#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> //ovo izbrisi posle
#include "ghost.h"

Ghost::Ghost(const float& speed,const std::string& color, bool boolGrid[28][36])
{
    this->speed = speed;
    this->color = color;
    animationNum = 0;
    movement.x = 0.0f;
    movement.y = 0.0f;
    for(int y=0;y<36;y++)
    {
        for(int x=0;x<28;x++)
        {
            this->boolGrid[x][y] = boolGrid[x][y];
        }
    }

    nextTile.x = -1;
    nextTile.y = -1;
    texture.loadFromFile("textures/" + color);
    texture2.loadFromFile("textures/blueGhost");
    body.setSize(sf::Vector2f(49.0f, 49.0f));
    body.setOrigin(body.getSize().x/2.0, body.getSize().y/2.0);
    body.setTexture(&texture);
    isScared = false;

    float offset = 0.0f;

    if(color == "pinkGhost")
    {
        offset = 28.0f;
    }
    if(color == "yellowGhost")
    {
        offset = 56.0f;
    }
    if(color == "greenGhost")
    {
        offset = 84.0f;
    }
    body.setPosition(350.0f + offset,420.0f);
}

Ghost::~Ghost()
{

}

void Ghost::Update(sf::RenderWindow& window, const float& deltaTime, float& counter, const sf::Vector2f& pacmanPosition, const sf::Vector2f& pacmanMovement, const sf::Vector2f& redGostPosition)
{
    // if(counter>0.1f)
    // {
    //         counter=0.0f;
    //         if(animationNum == 0)
    //         {
    //             body.setTexture(&texture);
    //             animationNum++;
    //         }
    //         else
    //         {
    //             body.setTexture(&texture2);
    //             animationNum = 0;
    //         }
    // }
    if(isScared)
    {
        body.setTexture(&texture2);
    }
    else
    {
        body.setTexture(&texture);
    }
    
    if(nextTile.x == -1 || nextTile.y == -1 || body.getPosition().x > nextTile.x*28.0f - 0.58*28.0f && body.getPosition().x < nextTile.x*28.0f - 0.42*28.0f && body.getPosition().y > nextTile.y*28.0f - 0.58f*28.0f && body.getPosition().y < nextTile.y*28.0f - 0.42f*28.0f)
    {
        sf::Vector2i current((int)body.getPosition().x / 28 + 1, (int)body.getPosition().y / 28 + 1);
        sf::Vector2i target((int)pacmanPosition.x / 28 + 1, (int)pacmanPosition.y / 28 + 1);
        adjustTarget(target, pacmanPosition, pacmanMovement, redGostPosition);
        nextTile = pathfind(current, target);
        //std::cout << nextTile.x << " " << nextTile.y << std::endl;
    }

    if(body.getPosition().x < nextTile.x*28.0f - 0.58*28.0f)
        {
            movement.x = speed*deltaTime;
            movement.y = 0.0f;
        }
    else if (body.getPosition().x > nextTile.x*28.0f - 0.42*28.0f)
        {
            movement.x = -speed*deltaTime;
            movement.y = 0.0f;
        }
    else if(body.getPosition().y < nextTile.y*28.0f - 0.58f*28.0f)
        {
            movement.x = 0.0f;
            movement.y = speed*deltaTime;
        }
    else if (body.getPosition().y > nextTile.y*28.0f - 0.42f*28.0f)
        {
            movement.x = 0.0f;
            movement.y = -speed*deltaTime;
        }
    body.move(movement.x, movement.y);
    //if(isScared)std::cout << "Scared" << std::endl;
    //std::cout << movement.x << " " << movement.y << " " << body.getPosition().x << " " << body.getPosition().y << std::endl;

    if(body.getPosition().x>window.getSize().x+body.getSize().x/2.0f)
    {
        body.setPosition(-body.getSize().x/2.0f, body.getPosition().y);
    }
    if(body.getPosition().x<-body.getSize().x/2.0f)
    {
        body.setPosition(window.getSize().x+body.getSize().x/2.0f, body.getPosition().y);
    }
}

void Ghost::Draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::Vector2f Ghost::getPosition()
{
    return body.getPosition();
}

sf::Vector2f Ghost::getSize()
{
    return body.getSize();
}


Collider Ghost::getCollider() 
{
    return Collider(body);
}

bool Ghost::getScared()
{
    return isScared;
}

void Ghost::setScared(const bool& isScared)
{
    this->isScared = isScared;
    if(isScared)
    {
        speed = 120.0f;
    }
    else
    {
        speed = 224.0f;
    }
    
}

void Ghost::setSpeed(const float& speed)
{
    this->speed = speed;
}

float distanceSquared(const float& x1, const float& y1, const float& x2, const float& y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

int distanceSquaredInt(const int& x1, const int& y1, const int& x2, const int& y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

void Ghost::adjustTarget(sf::Vector2i &target, const sf::Vector2f& pacmanPosition, const sf::Vector2f& pacmanMovement, const sf::Vector2f& redGhostPosition)
{
    //menjanje mete u zavisnosti od duha i moda
    if(isScared)
    {
        if(color == "redGhost")
        {
            target.x = 28;
            target.y = 5;
        }
        else if(color == "pinkGhost")
        {
            target.x = 1;
            target.y = 5;
        }
        else if(color == "yellowGhost")
        {
            target.x = 1;
            target.y = 28;
        }
        else if(color == "greenGhost")
        {
            target.x = 28;
            target.y = 28;
        }
    }
    else
    {
        if(color == "pinkGhost")
        {
            if(pacmanMovement.x == 0.0f) // kretanje vertikalno
            {
                if(pacmanMovement.y > 0.0f) // kretanje dole
                {
                    target.y += 4;
                }
                else if(pacmanMovement.y < 0.0f) // kretanje gore
                {
                    target.y -= 4;
                }
            }
            else if(pacmanMovement.y == 0.0f) // kretanje horizontalno
            {
                if(pacmanMovement.x > 0.0f) // kretanje desno
                {
                    target.x += 4;
                }
                else if(pacmanMovement.x < 0.0f) // kretanje levo
                {
                    target.x -= 4;
                }
            }
        }
        else if(color == "greenGhost")
        {
            if(distanceSquared(pacmanPosition.x, pacmanPosition.y, body.getPosition().x, body.getPosition().y) < (8.0f * 28.0f)*(8.0f * 28.0f))
            {
                target.x = 2;
                target.y = 33;
            }
        }
        else if(color == "yellowGhost")
        {
            sf::Vector2i redPos((int)redGhostPosition.x / 28 + 1, (int)redGhostPosition.y / 28 + 1);
            sf::Vector2i pacmanPos((int)pacmanPosition.x / 28 + 1, (int)pacmanPosition.y / 28 + 1);
            target.x = 2*pacmanPos.x - redPos.x;
            target.y = 2*pacmanPos.y - redPos.y;
        }
    }
}

std::vector<char> Ghost::validPaths(const int& x, const int& y)
{
    std::vector<char> v;
    if(x-1>=0 && boolGrid[x-1][y])//leva
    {
        v.push_back('L');
    }
    if(x+1<28 && boolGrid[x+1][y])//desna
    {
        v.push_back('R');
    }
    if(y-1>=0 && boolGrid[x][y-1])//gore
    {
        v.push_back('U');
    }
    if(x+1<36 && boolGrid[x][y+1])//dole
    {
        v.push_back('D');
    }
    return v;
}

sf::Vector2i Ghost::pathfind(const sf::Vector2i& start, const sf::Vector2i& target)
{
    sf::Vector2i nextTile(start.x, start.y);
    int min = __INT_MAX__;
    for(auto path : validPaths(start.x - 1, start.y - 1))
    {
        switch(path) 
        {
            case 'L':
                if(movement.x <= 0.0f && distanceSquaredInt(start.x - 1, start.y, target.x, target.y) < min)
                {
                    nextTile.x = start.x - 1;
                    nextTile.y = start.y;
                    min = distanceSquaredInt(start.x - 1, start.y, target.x, target.y);
                }
                break;
            case 'R':
                if(movement.x >= 0.0f && distanceSquaredInt(start.x + 1, start.y, target.x, target.y) < min)
                {
                    nextTile.x = start.x + 1;
                    nextTile.y = start.y;
                    min = distanceSquaredInt(start.x + 1, start.y, target.x, target.y);
                }
                break;
            case 'U':
                if(movement.y <= 0.0f && distanceSquaredInt(start.x, start.y - 1, target.x, target.y) < min)
                {
                    nextTile.x = start.x;
                    nextTile.y = start.y - 1;
                    min = distanceSquaredInt(start.x, start.y - 1, target.x, target.y);
                }
                break;
            case 'D':
                if(movement.y >= 0.0f && distanceSquaredInt(start.x, start.y + 1, target.x, target.y) < min)
                {
                    nextTile.x = start.x;
                    nextTile.y = start.y + 1;
                    min = distanceSquaredInt(start.x, start.y + 1, target.x, target.y);
                }
                break;
        }
    }
    return nextTile;
}

void Ghost::die()
{
    this->setScared(false);
    float offset = 0.0f;

    if(color == "pinkGhost")
    {
        offset = 28.0f;
    }
    if(color == "yellowGhost")
    {
        offset = 56.0f;
    }
    if(color == "greenGhost")
    {
        offset = 84.0f;
    }
    body.setPosition(350.0f + offset,420.0f);
    body.setTexture(&texture);
    nextTile.x = -1;
    nextTile.y = -1;
}
