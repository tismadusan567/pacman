#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib> 
#include <ctime>
#include "player.h"
#include "dot.h"
#include "wall.h"
#include "collider.h"
#include "ghost.h"

#define WINDOW_HEIGHT 1008
#define WINDOW_WIDTH 784
#define TILE_SIZE 28
#define BIGWALL 28.0f
#define SMALLWALL 14.0f

char grid[WINDOW_WIDTH/TILE_SIZE][WINDOW_HEIGHT/TILE_SIZE] = {0};
bool boolGrid[28][36];

std::vector<Wall> setWalls(sf::Texture* texture);
std::vector<Dot> setDots(sf::Texture* texture);
void setGrids();

int main()
{
    //window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PACMAN");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);


    sf::View view(sf::Vector2f(float(WINDOW_WIDTH/2), float(WINDOW_HEIGHT/2)),sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    window.setView(view);

    //textures
    sf::Texture dotTexture, PacmanTexture, PacmanTextureClosed, wallTexture, redGhostTexture;
    dotTexture.loadFromFile("pacmanClosed.png");
    PacmanTexture.loadFromFile("pacmanRight.png");
    PacmanTextureClosed.loadFromFile("pacmanClosed.png");
    wallTexture.loadFromFile("space.jpg");
    redGhostTexture.loadFromFile("red ghost.png");

    //font
    sf::Font font;
    font.loadFromFile("ARCADECLASSIC.TTF");

    //text
    sf::Text score("SCORE ", font);
    score.setCharacterSize(30);
    score.setStyle(sf::Text::Bold);
    score.setFillColor(sf::Color::White);

    //objects
    setGrids(); 
    Player player(280.0f, &PacmanTexture, &PacmanTextureClosed, boolGrid);
    Ghost redGhost(280.0f,sf::Vector2f((float)WINDOW_HEIGHT/2.0f, 18.0f*(float)TILE_SIZE), &redGhostTexture, &redGhostTexture);
    std::vector<Wall> walls=setWalls(&wallTexture);
    std::vector<Dot> dots=setDots(&dotTexture); 

    //variables
    float deltaTime = 0.0f;
    float fps;
    float playerCounter=0.0f,ghostCounter=0.0f;
    int scoreInt=0;
    sf::Clock clock;
    std::srand((unsigned)time(0)); 

    //game loop
    while (window.isOpen())
    {
        //deltaTime / fps
        deltaTime = clock.restart().asSeconds();
        fps = 1.f / deltaTime;
        if(fps<59.0f){
            std::cout << fps << std::endl;
        }

        //events
        sf::Event event; 
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    std::cout << event.size.width << "x" << event.size.height << std::endl;
                    view.setCenter(window.getView().getCenter());
                    view.setSize(sf::Vector2f(event.size.width, event.size.height));
                    window.setView(view);

                    break;
                case sf::Event::TextEntered:
                    if(event.text.unicode < 128){
                        std::cout << (char)event.text.unicode << std::flush;
                    }
            }
        }

        //update / draw / display
        playerCounter+=deltaTime;
        ghostCounter+=deltaTime;
        redGhost.setDirection(rand()%4);
        player.Update(window, deltaTime, playerCounter); //update player
        redGhost.Update(window, deltaTime, ghostCounter);

        Collider playerCollider = player.getCollider();
        for(int i=0;i<walls.size();i++){
            walls[i].getCollider().checkCollision(playerCollider, 1.0f);
        }
        Collider redGhostCollider = redGhost.getCollider();
        for(int i=0;i<walls.size();i++){
            walls[i].getCollider().checkCollision(redGhostCollider, 1.0f);
        }

        for(int i=0;i<dots.size();i++){ //update dots
            if(dots[i].getActive()==false)continue;
            if(abs(player.getPosition().x - dots[i].getPosition().x)<(player.getSize().x/2.0f + dots[i].getSize().x/2.0f) && abs(player.getPosition().y - dots[i].getPosition().y)<(player.getSize().y/2.0f + dots[i].getSize().y/2.0f)){
                dots[i].setActive(false);
                scoreInt+=dots[i].getPoints();
            }
        }

        std::string scoreString = "SCORE "; //update score
        scoreString += std::to_string(scoreInt);
        score.setString(scoreString);

        window.clear();
        player.Draw(window);
        //std::cout << player.getPosition().x << " " << player.getPosition().y << std::endl;//
        for(int i=0;i<walls.size();i++){
            walls[i].Draw(window);
        }
        for(int i=0;i<dots.size();i++){
            if(dots[i].getActive())dots[i].Draw(window);
        }
        redGhost.Draw(window);
        window.draw(score);
        window.display();
    }

    return 0;
}

std::vector<Dot> setDots(sf::Texture* texture)
{
    std::vector<Dot> v;
    for(int x=0;x<WINDOW_WIDTH;x+=TILE_SIZE){
        for(int y=0;y<WINDOW_HEIGHT;y+=TILE_SIZE){
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='d'){
                Dot temp((float)x+float(TILE_SIZE/2),(float)y+float(TILE_SIZE/2), texture, 7.0f, 10);
                v.push_back(temp);
            }
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='D'){
                Dot temp((float)x+float(TILE_SIZE/2),(float)y+float(TILE_SIZE/2), texture, 28.0f, 50);
                v.push_back(temp);
            }
        }
    }
    return v;
}

std::vector<Wall> setWalls(sf::Texture* texture)
{
    std::vector<Wall> v;

    //unutrasnji zidovi
    for(int x=0;x<WINDOW_WIDTH;x+=TILE_SIZE){
        for(int y=0;y<WINDOW_HEIGHT;y+=TILE_SIZE){
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='S'){
                sf::Vector2f size(0.0f, 0.0f), position;
                for(int i=x+TILE_SIZE;grid[i/TILE_SIZE][y/TILE_SIZE]=='W' || grid[i/TILE_SIZE][y/TILE_SIZE]=='E';i+=TILE_SIZE)
                {
                    size.x+=BIGWALL;
                }
                for(int j=y+TILE_SIZE;grid[x/TILE_SIZE][j/TILE_SIZE]=='W' || grid[x/TILE_SIZE][j/TILE_SIZE]=='E';j+=TILE_SIZE)
                {
                    size.y+=BIGWALL;
                }
                if(size.x == 0.0f)size.x=BIGWALL;
                if(size.y == 0.0f)size.y=BIGWALL;
                position.x = (float)x + size.x/2.0f + TILE_SIZE/2.0f;
                position.y = (float)y + size.y/2.0f + TILE_SIZE/2.0f;
                Wall temp(size, position, texture);
                v.push_back(temp);
            }
        }
    }   

    //spoljasnji zidovi

    //vertical
    Wall temp(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f), texture);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 10.0f*BIGWALL), sf::Vector2f(SMALLWALL/2.0f, 3*TILE_SIZE+BIGWALL*10.0f/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 10.0f*BIGWALL), sf::Vector2f((float)WINDOW_WIDTH-SMALLWALL/2.0f, 3*TILE_SIZE+BIGWALL*10.0f/2.0f), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(SMALLWALL, 12.0f*BIGWALL), sf::Vector2f(SMALLWALL/2.0f, (float)WINDOW_HEIGHT-2.0f*TILE_SIZE-BIGWALL*12.0f/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 12.0f*BIGWALL), sf::Vector2f((float)WINDOW_WIDTH-SMALLWALL/2.0f,  (float)WINDOW_HEIGHT-2.0f*TILE_SIZE-BIGWALL*12.0f/2.0f), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(SMALLWALL, 3.0f*TILE_SIZE), sf::Vector2f(5*TILE_SIZE+SMALLWALL/2.0f, 14.5f*TILE_SIZE), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 3.0f*TILE_SIZE), sf::Vector2f((float)WINDOW_WIDTH-(5*TILE_SIZE+SMALLWALL/2.0f),  14.5f*TILE_SIZE), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(SMALLWALL, 3.0f*TILE_SIZE), sf::Vector2f(5*TILE_SIZE+SMALLWALL/2.0f, 20.5f*TILE_SIZE), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 3.0f*TILE_SIZE), sf::Vector2f((float)WINDOW_WIDTH-(5*TILE_SIZE+SMALLWALL/2.0f),  20.5f*TILE_SIZE), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(SMALLWALL, 4.0f*TILE_SIZE), sf::Vector2f(11.0f*TILE_SIZE-SMALLWALL/2.0f, 17.0f*TILE_SIZE+SMALLWALL), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(SMALLWALL, 4.0f*TILE_SIZE), sf::Vector2f((float)WINDOW_WIDTH-(11.0f*TILE_SIZE-SMALLWALL/2.0f),  17.0f*TILE_SIZE+SMALLWALL), texture);v.push_back(temp);


    //horizontal
    temp.setAttributes(sf::Vector2f(5*TILE_SIZE, SMALLWALL), sf::Vector2f(3*TILE_SIZE, 13*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(5*TILE_SIZE, SMALLWALL), sf::Vector2f(WINDOW_WIDTH-3*TILE_SIZE, 13*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f((5*TILE_SIZE + SMALLWALL)/2.0f, 19*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f(WINDOW_WIDTH-(5*TILE_SIZE + SMALLWALL)/2.0f, 19*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f((5*TILE_SIZE + SMALLWALL)/2.0f, 17*TILE_SIZE-3.0f/4.0f*TILE_SIZE), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f(WINDOW_WIDTH-(5*TILE_SIZE + SMALLWALL)/2.0f, 17*TILE_SIZE-3.0f/4.0f*TILE_SIZE), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f((5*TILE_SIZE + SMALLWALL)/2.0f, 23*TILE_SIZE-3.0f/4.0f*TILE_SIZE), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(5*TILE_SIZE + SMALLWALL, SMALLWALL), sf::Vector2f(WINDOW_WIDTH-(5*TILE_SIZE + SMALLWALL)/2.0f, 23*TILE_SIZE-3.0f/4.0f*TILE_SIZE), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f(7*TILE_SIZE, SMALLWALL), sf::Vector2f(14.0f*TILE_SIZE, 16*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f(7*TILE_SIZE, SMALLWALL), sf::Vector2f(14.0f*TILE_SIZE, 19*TILE_SIZE+SMALLWALL/2.0f), texture);v.push_back(temp);

    temp.setAttributes(sf::Vector2f((float)WINDOW_WIDTH, SMALLWALL), sf::Vector2f((float)WINDOW_WIDTH/2.0f, 3*TILE_SIZE+SMALLWALL/2.0f), texture);v.push_back(temp);
    temp.setAttributes(sf::Vector2f((float)WINDOW_WIDTH, SMALLWALL), sf::Vector2f((float)WINDOW_WIDTH/2.0f, (float)WINDOW_HEIGHT-2*TILE_SIZE-SMALLWALL/2.0f), texture);v.push_back(temp);
    return v;
}

void setGrids()
{
    std::ifstream in("gridAll");
    for(int y=0;y<WINDOW_HEIGHT/TILE_SIZE;y++){
        for(int x=0;x<WINDOW_WIDTH/TILE_SIZE;x++){
            in >> grid[x][y];
        }
    }
    std::ifstream in2("boolGrid");
    for(int y=0;y<36;y++){
        for(int x=0;x<28;x++){
            in2 >> boolGrid[x][y];
        }
    }
}