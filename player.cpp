#include <SFML/Graphics.hpp>
#include "player.h"

Player::Player(float speed, sf::Texture* texture, sf::Texture* texture2)
{
    this->speed = speed;
    this->texture = texture;
    this->texture2 = texture2;
    isClosed = false;
    movement.x = 0.0f;
    movement.y = 0.0f;

    body.setSize(sf::Vector2f(49.0f, 49.0f));
    body.setOrigin(body.getSize().x/2.0, body.getSize().y/2.0);
    body.setPosition(392.0f, 728.0f);
    body.setTexture(texture);
}

Player::~Player()
{

}

void Player::Update(sf::RenderWindow& window, float deltaTime, float& counter)
{
    if(counter>0.1f){
            counter=0.0f;
            if(isClosed){
                body.setTexture(texture);
                isClosed = false;
            }
            else{
                body.setTexture(texture2);
                isClosed = true;
            }
        }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        movement.x = -speed*deltaTime;
        movement.y = 0.0f;
        body.setRotation(180.f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        movement.x = speed*deltaTime;
        movement.y = 0.0f;
        body.setRotation(0.f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        movement.x = 0.0f;
        movement.y = -speed*deltaTime;
        body.setRotation(270.f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        movement.x = 0.0f;
        movement.y = speed*deltaTime;
        body.setRotation(90.f);
    }
    body.move(movement.x, movement.y);
    if(body.getPosition().x>window.getSize().x+body.getSize().x/2.0f){
        body.setPosition(-body.getSize().x/2.0f, body.getPosition().y);
    }
    if(body.getPosition().x<-body.getSize().x/2.0f){
        body.setPosition(window.getSize().x+body.getSize().x/2.0f, body.getPosition().y);
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


Collider Player::getCollider() 
{
    return Collider(body);
}
