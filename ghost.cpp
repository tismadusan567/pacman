#include <SFML/Graphics.hpp>
#include "ghost.h"

Ghost::Ghost(float speed,sf::Vector2f position, sf::Texture* texture, sf::Texture* texture2)
{
    this->speed = speed;
    this->texture = texture;
    this->texture2 = texture2;
    animationNum = 0;
    movement.x = 0.0f;
    movement.y = 0.0f;

    body.setSize(sf::Vector2f(49.0f, 49.0f));
    body.setOrigin(body.getSize().x/2.0, body.getSize().y/2.0);
    body.setPosition(position);
    body.setTexture(texture);
}

Ghost::~Ghost()
{

}

void Ghost::Update(sf::RenderWindow& window, float deltaTime, float& counter)
{
    if(counter>0.1f){
            counter=0.0f;
            if(animationNum == 0){
                body.setTexture(texture);
                animationNum++;
            }
            else{
                body.setTexture(texture2);
                animationNum = 0;
            }
        }    
    if(randDirection==0){
        movement.x = -speed*deltaTime;
        movement.y = 0.0f;
    }
    if(randDirection==1){
        movement.x = speed*deltaTime;
        movement.y = 0.0f;
    }
    if(randDirection==2){
        movement.x = 0.0f;
        movement.y = -speed*deltaTime;
    }
    if(randDirection==3){
        movement.x = 0.0f;
        movement.y = speed*deltaTime;
    }
    body.move(movement.x, movement.y);
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
