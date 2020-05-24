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
bool boolGridGhosts[28][36];

std::vector<Wall> setWalls(sf::Texture* texture);
std::vector<Dot> setDots(sf::Texture* texture);
void setGrids();
void reset(Player& player, Ghost& red, Ghost& pink, Ghost& yellow, Ghost& green, std::vector<Dot>& dots, float& pDTime, float& rDTime, float& piDTime, float& yDTime, float& gDTime);

int main()
{
    //window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PACMAN");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);


    sf::View view(sf::Vector2f(float(WINDOW_WIDTH/2), float(WINDOW_HEIGHT/2)),sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    window.setView(view);

    //textures
    sf::Texture dotTexture, wallTexture;
    dotTexture.loadFromFile("textures/pacmanClosed.png");
    wallTexture.loadFromFile("textures/space.jpg");

    //font
    sf::Font font;
    font.loadFromFile("ARCADECLASSIC.TTF");

    //text
    sf::Text score("SCORE ", font);
    sf::Text livesText("LIVES", font);
    sf::Text levelText("LEVEL", font);
    sf::Text pauseText("PRESS SPACE TO CONTINUE", font);

    score.setCharacterSize(30);
    score.setStyle(sf::Text::Bold);
    score.setFillColor(sf::Color::White);

    livesText.setCharacterSize(30);
    livesText.setStyle(sf::Text::Bold);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(660.0f, 0.0f);

    levelText.setCharacterSize(30);
    levelText.setStyle(sf::Text::Bold);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(0.0f, 950.0f);

    pauseText.setCharacterSize(30);
    pauseText.setStyle(sf::Text::Bold);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(200.0f, 460.0f);

    //objects
    setGrids(); 
    Player player(280.0f, boolGrid);
    Ghost redGhost(200.0f,"redGhost", boolGridGhosts);
    Ghost pinkGhost(200.0f,"pinkGhost", boolGridGhosts);
    Ghost yellowGhost(200.0f,"yellowGhost", boolGridGhosts);
    Ghost greenGhost(200.0f,"greenGhost", boolGridGhosts);

    std::vector<Wall> walls=setWalls(&wallTexture);
    std::vector<Dot> dots=setDots(&dotTexture); 

    sf::RectangleShape crno, crno2;
    crno.setSize(sf::Vector2f(70.0f, 70.0f));
    crno.setFillColor(sf::Color::Black);
    crno.setPosition(784.0f,453.0f);
    crno2.setSize(sf::Vector2f(70.0f, 70.0f));
    crno2.setFillColor(sf::Color::Black);
    crno2.setPosition(-70.0f, 453.0f);

    //variables
    bool isPaused = true;
    float deltaTime = 0.0f;
    float fps;
    float playerCounter=0.0f,ghostCounter=0.0f;
    int scoreInt=0;
    sf::Clock clock;
    std::srand((unsigned)time(0)); 
    float timeScared = 7.0f;
    bool areScared = false;
    float redDeadTime = 0.0f;
    float pinkDeadTime = 0.0f;
    float yellowDeadTime = 0.0f;
    float greenDeadTime = 0.0f;
    float pacmanDeadTime = 0.0f;
    int lives = 3;
    int level = 1;

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
                    view.setCenter(window.getView().getCenter());
                    view.setSize(sf::Vector2f(event.size.width, event.size.height));
                    window.setView(view);

                    break;
                case sf::Event::KeyReleased:
                    if(event.key.code == sf::Keyboard::Key::Escape || event.key.code == sf::Keyboard::Key::Space){
                        isPaused = !isPaused;
                    }
            }
        }

        //update / draw / display
        if(!isPaused)
        {
            playerCounter+=deltaTime;
            ghostCounter+=deltaTime;

            if(!lives)
            {
                lives = 3;
                scoreInt = 0;
                reset(player, redGhost, pinkGhost, yellowGhost, greenGhost, dots, pacmanDeadTime, redDeadTime, pinkDeadTime, yellowDeadTime, greenDeadTime);
                isPaused = true;
                level = 1;
            }

            int activeDots = 0;
            for(int i=0;i<dots.size();i++)
            {
                if(dots[i].getActive())
                {
                    activeDots++;
                }
            }
            if(!activeDots)
            {
                level++;
                reset(player, redGhost, pinkGhost, yellowGhost, greenGhost, dots, pacmanDeadTime, redDeadTime, pinkDeadTime, yellowDeadTime, greenDeadTime);
            }

            if(areScared)
            {
                timeScared -= deltaTime;
                if(timeScared <= 0.0f)
                {
                    redGhost.setScared(false);
                    pinkGhost.setScared(false);
                    yellowGhost.setScared(false);
                    greenGhost.setScared(false);
                    areScared = false;
                    timeScared = 7.0f;
                }
            }
            if(pacmanDeadTime <= 0.0f)
            {
            player.Update(window, deltaTime, playerCounter); //update player
            }
            else
            {
                pacmanDeadTime -= deltaTime;
            }
            
            if(redDeadTime <= 0.0f)
            {  
                redGhost.Update(window, deltaTime, ghostCounter, player.getPosition(), player.getMovement(), redGhost.getPosition());
            }
            else
            {
                redDeadTime -= deltaTime;
            }

            if(pinkDeadTime <= 0.0f)
            {
                pinkGhost.Update(window, deltaTime, ghostCounter, player.getPosition(), player.getMovement(), redGhost.getPosition());
            }
            else
            {
                pinkDeadTime -= deltaTime;
            }

            if(yellowDeadTime <= 0.0f)
            {
                yellowGhost.Update(window, deltaTime, ghostCounter, player.getPosition(), player.getMovement(), redGhost.getPosition());
            }
            else
            {
                yellowDeadTime -= deltaTime;
            }

            if(greenDeadTime <= 0.0f)
            {
                greenGhost.Update(window, deltaTime, ghostCounter, player.getPosition(), player.getMovement(), redGhost.getPosition());
            }
            else
            {
                greenDeadTime -= deltaTime;
            }
            
            Collider playerCollider = player.getCollider();
            Collider redGhostCollider = redGhost.getCollider();
            Collider pinkGhostCollider = pinkGhost.getCollider();
            Collider yellowGhostCollider = yellowGhost.getCollider();
            Collider greenGhostCollider = greenGhost.getCollider();
            for(int i=0;i<walls.size();i++)
            {
                walls[i].getCollider().checkCollision(playerCollider, 1.0f);
                walls[i].getCollider().checkCollision(redGhostCollider, 1.0f);
                walls[i].getCollider().checkCollision(pinkGhostCollider, 1.0f);
                walls[i].getCollider().checkCollision(yellowGhostCollider, 1.0f);
                walls[i].getCollider().checkCollision(greenGhostCollider, 1.0f);
            }
            if(playerCollider.checkCollision(redGhostCollider, 0.5f))
            {
                if(redGhost.getScared())
                {
                    scoreInt += 400;
                }
                else
                {
                    player.die();pacmanDeadTime = 4.0f;
                    lives--;
                    pinkGhost.die();pinkDeadTime = 4.0f;
                    yellowGhost.die();yellowDeadTime = 4.0f;
                    greenGhost.die();greenDeadTime = 4.0f;
                }
                redGhost.die();
                redDeadTime = 4.0f;
            }
            if(playerCollider.checkCollision(pinkGhostCollider, 0.5f))
            {
                if(pinkGhost.getScared())
                {
                    scoreInt += 400;
                }
                else
                {
                    player.die();pacmanDeadTime = 4.0f;
                    lives--;
                    redGhost.die();redDeadTime = 4.0f;
                    yellowGhost.die();yellowDeadTime = 4.0f;
                    greenGhost.die();greenDeadTime = 4.0f;
                }
                pinkGhost.die();
                pinkDeadTime = 4.0f;
            }
            if(playerCollider.checkCollision(yellowGhostCollider, 0.5f))
            {
                if(yellowGhost.getScared())
                {
                    scoreInt += 400;
                }
                else
                {
                    player.die();pacmanDeadTime = 4.0f;
                    lives--;
                    redGhost.die();redDeadTime = 4.0f;
                    pinkGhost.die();pinkDeadTime = 4.0f;
                    greenGhost.die();greenDeadTime = 4.0f;
                }
                yellowGhost.die();
                yellowDeadTime = 4.0f;
            }
            if(playerCollider.checkCollision(greenGhostCollider, 0.5f))
            {
                if(greenGhost.getScared())
                {
                    scoreInt += 400;
                }
                else
                {
                    player.die();pacmanDeadTime = 4.0f;
                    lives--;
                    redGhost.die();redDeadTime = 4.0f;
                    yellowGhost.die();yellowDeadTime = 4.0f;
                    pinkGhost.die();pinkDeadTime = 4.0f;
                }
                greenGhost.die();
                greenDeadTime = 4.0f;
            }

            for(int i=0;i<dots.size();i++)//update dots
            { 
                if(dots[i].getActive()==false)continue;
                if(abs(player.getPosition().x - dots[i].getPosition().x)<(player.getSize().x/2.0f + dots[i].getSize().x/2.0f) && abs(player.getPosition().y - dots[i].getPosition().y)<(player.getSize().y/2.0f + dots[i].getSize().y/2.0f))
                {
                    dots[i].setActive(false);
                    scoreInt+=dots[i].getPoints();
                    if(dots[i].getBig())
                    {
                        redGhost.setScared(true);
                        pinkGhost.setScared(true);
                        yellowGhost.setScared(true);
                        greenGhost.setScared(true);
                        areScared = true;
                        timeScared = 7.0f;
                    }
                }
            }

            std::string scoreString = "SCORE "; //update score
            scoreString += std::to_string(scoreInt);
            score.setString(scoreString);

            std::string livesString = "LIVES "; //update lives
            livesString += std::to_string(lives);
            livesText.setString(livesString);

            std::string levelString = "LEVEL "; //update level
            levelString += std::to_string(level);
            levelText.setString(levelString);
        }

        window.clear();
        player.Draw(window);
        for(int i=0;i<walls.size();i++)
        {
            walls[i].Draw(window);
        }
        for(int i=0;i<dots.size();i++)
        {
            if(dots[i].getActive())dots[i].Draw(window);
        }
        redGhost.Draw(window);
        pinkGhost.Draw(window);
        yellowGhost.Draw(window);
        greenGhost.Draw(window);
        window.draw(score);
        window.draw(livesText);
        window.draw(levelText);
        window.draw(crno);
        window.draw(crno2);
        if(isPaused)
        {
            window.draw(pauseText);
        }
        window.display();
    }

    return 0;
}

std::vector<Dot> setDots(sf::Texture* texture)
{
    std::vector<Dot> v;
    for(int x=0;x<WINDOW_WIDTH;x+=TILE_SIZE)
    {
        for(int y=0;y<WINDOW_HEIGHT;y+=TILE_SIZE)
        {
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='d')
            {
                Dot temp((float)x+float(TILE_SIZE/2),(float)y+float(TILE_SIZE/2), texture, false);
                v.push_back(temp);
            }
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='D')
            {
                Dot temp((float)x+float(TILE_SIZE/2),(float)y+float(TILE_SIZE/2), texture, true);
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
    for(int x=0;x<WINDOW_WIDTH;x+=TILE_SIZE)
    {
        for(int y=0;y<WINDOW_HEIGHT;y+=TILE_SIZE)
        {
            if(grid[x/TILE_SIZE][y/TILE_SIZE]=='S')
            {
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
    for(int y=0;y<WINDOW_HEIGHT/TILE_SIZE;y++)
    {
        for(int x=0;x<WINDOW_WIDTH/TILE_SIZE;x++)
        {
            in >> grid[x][y];
        }
    }
    std::ifstream in2("boolGrid");
    for(int y=0;y<36;y++)
    {
        for(int x=0;x<28;x++)
        {
            in2 >> boolGrid[x][y];
        }
    }
    std::ifstream in3("boolGridGhosts");
    for(int y=0;y<36;y++)
    {
        for(int x=0;x<28;x++)
        {
            in3 >> boolGridGhosts[x][y];
        }
    }
}

void reset(Player& player, Ghost& red, Ghost& pink, Ghost& yellow, Ghost& green, std::vector<Dot>& dots, float& pDTime, float& rDTime, float& piDTime, float& yDTime, float& gDTime)
{
    player.die();pDTime = 4.0f;
    red.die();rDTime = 4.0f;
    yellow.die();yDTime = 4.0f;
    pink.die();piDTime = 4.0f;
    green.die();gDTime = 4.0f;

    for(int i=0;i < dots.size();i++)
    {
        dots[i].setActive(true);
    }
}