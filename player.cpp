#include <SFML/Graphics.hpp>
#include <iostream> //ovo izbrisi posle
#include "player.h"

Player::Player(float speed, bool boolGrid[28][36])
{
    this->speed = speed;
    
    for(int y=0;y<36;y++){
        for(int x=0;x<28;x++){
            this->boolGrid[x][y] = boolGrid[x][y];
        }
    }
    texture.loadFromFile("textures/pacmanRight.png");
    texture2.loadFromFile("textures/pacmanClosed.png");

    nextDirection = 's';
    isClosed = false;
    movement.x = 0.0f;
    movement.y = 0.0f;

    body.setSize(sf::Vector2f(49.0f, 49.0f));
    body.setOrigin(body.getSize().x/2.0, body.getSize().y/2.0);
    body.setPosition(392.0f, 742.0f);
    body.setTexture(&texture);
}

Player::~Player()
{

}

void Player::Update(sf::RenderWindow& window, float deltaTime, float& counter)
{
    if(counter>0.1f){
            counter=0.0f;
            if(isClosed){
                body.setTexture(&texture);
                isClosed = false;
            }
            else{
                body.setTexture(&texture2);
                isClosed = true;
            }
        }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        nextDirection = 'L';
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        nextDirection = 'R';
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        nextDirection = 'U';
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        nextDirection = 'D';
    }

    int currentTileX = (int)body.getPosition().x / 28 ;
    int currentTileY = (int)body.getPosition().y / 28 ;
    switch(nextDirection)
    {
        case 'L':
            if(boolGrid[currentTileX - 1][currentTileY] && body.getPosition().y < currentTileY *28.0f + 0.6f*28.0f && body.getPosition().y > currentTileY*28.0f + 0.4*28.0f)
            {
                movement.x = -speed*deltaTime;
                movement.y = 0.0f;
                body.setRotation(180.f);
            }
            break;
        case 'R':
            if(boolGrid[currentTileX + 1][currentTileY] && body.getPosition().y < currentTileY *28.0f + 0.6f*28.0f && body.getPosition().y > currentTileY*28.0f + 0.4*28.0f)
            {
                movement.x = speed*deltaTime;
                movement.y = 0.0f;
                body.setRotation(0.f);
            }
            break;
        case 'U':
            if(boolGrid[currentTileX][currentTileY-1] && body.getPosition().x < currentTileX *28.0f + 0.6f*28.0f && body.getPosition().x > currentTileX*28.0f + 0.4f*28.0f)
            {
                movement.x = 0.0f;
                movement.y = -speed*deltaTime;
                body.setRotation(270.f);
            }
            break;
        case 'D':
            if(boolGrid[currentTileX][currentTileY+1] && body.getPosition().x < currentTileX *28.0f + 0.6f*28.0f && body.getPosition().x > currentTileX*28.0f + 0.4*28.0f)
            {
                movement.x = 0.0f;
                movement.y = speed*deltaTime;
                body.setRotation(90.f);
            }
            break;
    }
    //std::cout << movement.x << " " << movement.y << std::endl;
    body.move(movement.x, movement.y);
    if(body.getPosition().x > 784.0f + body.getSize().x / 2.0f){
        body.setPosition(-body.getSize().x / 2.0f, body.getPosition().y);
    }
    if(body.getPosition().x < -body.getSize().x/2.0f){
        body.setPosition(784.0f + body.getSize().x/2.0f, body.getPosition().y);
    }
}

void Player::Draw(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::Vector2f Player::getPosition()
{
    return body.getPosition();
}

sf::Vector2f Player::getSize()
{
    return body.getSize();
}

sf::Vector2f Player::getMovement()
{
    return movement;
}

Collider Player::getCollider() 
{
    return Collider(body);
}

void Player::die()
{
    body.setPosition(392.0f, 742.0f);
    body.setTexture(&texture);
}
