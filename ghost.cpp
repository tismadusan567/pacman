#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <iostream> //ovo izbrisi posle
#include "ghost.h"

Ghost::Ghost(float speed,sf::Vector2f position, bool boolGrid[28][36])
{
    this->speed = speed;
    animationNum = 0;
    movement.x = 0.0f;
    movement.y = 0.0f;
    for(int y=0;y<36;y++){
        for(int x=0;x<28;x++){
            this->boolGrid[x][y] = boolGrid[x][y];
        }
    }

    nextTile.x = 19;
    nextTile.y = 20;
    texture.loadFromFile("red ghost.png");
    texture2.loadFromFile("red ghost.png");
    body.setSize(sf::Vector2f(49.0f, 49.0f));
    body.setOrigin(body.getSize().x/2.0, body.getSize().y/2.0);
    body.setPosition(position);
    body.setTexture(&texture);
}

Ghost::~Ghost()
{

}

void Ghost::Update(sf::RenderWindow& window, float deltaTime, float& counter)
{
    if(counter>0.1f){
            counter=0.0f;
            if(animationNum == 0){
                body.setTexture(&texture);
                animationNum++;
            }
            else{
                body.setTexture(&texture2);
                animationNum = 0;
            }
        }    
    
    int currentTileX = (int)body.getPosition().x / 28 ;
    int currentTileY = (int)body.getPosition().y / 28 ;

    if(body.getPosition().x > nextTile.x *28.0f - 0.58*28.0f && body.getPosition().x < nextTile.x *28.0f - 0.42*28.0f){
        if(body.getPosition().y < nextTile.y *28.0f - 0.58f*28.0f)
        {
            movement.x = 0.0f;
            movement.y = speed*deltaTime;
        }
        else if (body.getPosition().y > nextTile.y*28.0f - 0.42f*28.0f)
        {
            movement.x = 0.0f;
            movement.y = -speed*deltaTime;
        }
        else
        {
            sf::Vector2i current(currentTileX+1,currentTileY+1);
            sf::Vector2i target(27,5);

            nextTile = bfs(current, target).at(1);
            std::cout << nextTile.x << " " << nextTile.y << std::endl;
        }
    }
    else
    {
        if(body.getPosition().x < nextTile.x *28.0f - 0.58*28.0f)
        {
            movement.x = speed*deltaTime;
            movement.y = 0.0f;
        }
        else if (body.getPosition().x > nextTile.x*28.0f - 0.42*28.0f)
        {
            movement.x = -speed*deltaTime;
            movement.y = 0.0f;
        }
        else
        {
            sf::Vector2i current(currentTileX+1,currentTileY+1);
            sf::Vector2i target(27,5);

            nextTile = bfs(current, target).at(1);
            std::cout << nextTile.x << " " << nextTile.y << std::endl;
        }
    }
    
    body.move(movement.x, movement.y);
    std::cout << movement.x << " " << movement.y << " " << body.getPosition().x << " " << body.getPosition().y << std::endl;

    if(body.getPosition().x>window.getSize().x+body.getSize().x/2.0f){
        body.setPosition(-body.getSize().x/2.0f, body.getPosition().y);
    }
    if(body.getPosition().x<-body.getSize().x/2.0f){
        body.setPosition(window.getSize().x+body.getSize().x/2.0f, body.getPosition().y);
    }

}

void Ghost::Draw(sf::RenderWindow& window)
{
    window.draw(body);
}

void Ghost::setDirection(int randDirection)
{
    this->randDirection=randDirection;
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

std::vector<sf::Vector2i> Ghost::bfs(sf::Vector2i start, sf::Vector2i target)
{
    std::queue<std::pair<sf::Vector2i, std::vector<sf::Vector2i>>> q;
    bool boolGridCopy[29][37] = {0};
    for(int y=1;y<37;y++){
        for(int x=1;x<29;x++){
            boolGridCopy[x][y] = boolGrid[x-1][y-1];
        }
    }
    std::vector<sf::Vector2i> v;
    q.push(std::make_pair(start, v));
    boolGridCopy[start.x][start.y] = 0;
    while(!q.empty())
    {
        std::cout << 1 << std::flush;
        sf::Vector2i current = q.front().first;
        std::vector<sf::Vector2i> path = q.front().second;
        q.pop();
        boolGridCopy[current.x][current.y] = 0;
        path.push_back(current);

        if (current == target)
        {
            return path;
        }

        if(boolGridCopy[current.x-1][current.y])//leva
        {
            sf::Vector2i leva = current;
            leva.x-=1;
            q.push(std::make_pair(leva, path));
        }
        if(boolGridCopy[current.x+1][current.y])//desna
        {
            sf::Vector2i desna = current;
            desna.x+=1;
            q.push(std::make_pair(desna, path));
        }
        if(boolGridCopy[current.x][current.y-1])//gornja
        {
            sf::Vector2i gornja = current;
            gornja.y-=1;
            q.push(std::make_pair(gornja, path));
        }
        if(boolGridCopy[current.x][current.y+1])//donja
        {
            sf::Vector2i donja = current;
            donja.y+=1;
            q.push(std::make_pair(donja, path));
        }
    }
    return v;

}