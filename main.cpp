#define SFML_STATIC
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include <conio.h>
#include "snake.h"
#include <cmath>
#include <cstdlib>
#include <crtdbg.h>
#include <vector>

using namespace std;
using namespace sf;

/* Documentation :

Unique Feature is Audio:
    I personaly made this music of beats in the main menu and rattling sound in the actual game.
    Also, if you collect targets, there is a chime sound.

Scoring System    
    Moreover, this has an interesting scoring method where you get negative score when time ticks. This is because you are taking
    a lot of time to get the target. If you get target then you earn a 1000 point but since you took some time, there will be deductions in the total of 2000 points.
*/


int main()
{
    Wrapper();
    _CrtDumpMemoryLeaks();
}

void Wrapper() {

    int numberOfTargets = 0;
    int score = 0;
    bool startScreen = true;

    // Getting Audios
    SoundBuffer gameBuffer;
    if (!gameBuffer.loadFromFile("gameAudio.wav")) {
        cout << "No Audio found" << endl;
    }
    SoundBuffer mainMenuBuffer;
    if (!mainMenuBuffer.loadFromFile("mainMenu.wav")) {
        cout << "No Audio Found" << endl;
    }
    Sound gameSound;
    gameSound.setBuffer(gameBuffer);

    Sound mainMenuSound;
    mainMenuSound.setBuffer(mainMenuBuffer);


    // Creating Screen
    sf::RenderWindow window(sf::VideoMode(600, 600), "Gravity Snake!");

    // Seeding
    srand(time(NULL));
    // Creating World
    b2Vec2 gravity(0.0f, -0.5f);
    b2World world(gravity);
    
    // Creating Walls
    InitializeWallsObjects(&world);

    
    // Creating Visible Walls
    RectangleShape leftWall(Vector2f(20, window.getSize().x));
    RectangleShape rightWall(Vector2f(20, window.getSize().x));
    RectangleShape topWall(Vector2f(window.getSize().x, 20));
    RectangleShape bottomWall(Vector2f(window.getSize().x, 20));
    
    // Setting Positions
    rightWall.setPosition(600-20, 0);
    bottomWall.setPosition(0,600-20);
    // Assisgning colors
    leftWall.setFillColor(Color(125,185,240,200));
    rightWall.setFillColor(Color(125, 185, 240, 200));
    topWall.setFillColor(Color(125, 185, 240, 200));
    bottomWall.setFillColor(Color(125, 185, 240, 200));

    // Creating Text
    Font font;
    Text startingText;
    Text headingText;
    Text scoreText;
    if (!font.loadFromFile("Raleway-Light.ttf")) {
        cout << "no font" << endl;
    }
    headingText.setFont(font);
    headingText.setString("GRAVITY SNAKE");
    headingText.setPosition(110, 20);
    headingText.setCharacterSize(48);
    headingText.setFillColor(Color::Red);
    startingText.setFont(font);
    startingText.setString("Welcome to Gravity Snake! Collect yellow dots! \n Move left by pressing \'A\' and right by \'D' \nand also Can go up by pressing \'W\'\n Press Space to Start!");
    startingText.setCharacterSize(20);
    startingText.setPosition(100, 250);
    startingText.setOutlineColor(Color::Black);
    startingText.setFillColor(Color(125, 185, 240, 100));
    scoreText.setFont(font);
    scoreText.setString("Score: "+ to_string(score));
    scoreText.setPosition(20, 30);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(Color::Cyan);

    // Creating Snake
    b2BodyDef snakeBodyDef;
    snakeBodyDef.type = b2_dynamicBody;
    snakeBodyDef.position.Set(0.0f, 0.0f);
    b2Body* snakeBody = world.CreateBody(&snakeBodyDef);
    b2PolygonShape snakeBox;
    snakeBox.SetAsBox(2.5f, 15.0f);
    b2FixtureDef snakeFixture;
    snakeFixture.shape = &snakeBox;
    snakeFixture.density = 0.20f;
    snakeBody->CreateFixture(&snakeFixture);

    // Creating Snake Object(Visible)
    RectangleShape snakeObject(Vector2f(5, 30));


    // Creating Target
    b2Vec2* target = new b2Vec2();
    float LO = -275.0f;
    float HI = 275.0f;
    float randomNumber1 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
    float randomNumber2 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

    // Creating Target Object (Visible)
    CircleShape targetObject(4.0f);
    targetObject.setFillColor(Color::Yellow);
    
    // setting target
    target->Set(randomNumber1 , randomNumber2);
    targetObject.setPosition(300+target->x, 300-target->y);
  
    // SFML
    while (window.isOpen())
    {
        Event event;
       
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
                window.close();
            if (startScreen) {
                mainMenuSound.play();
                mainMenuSound.setLoop(true);
                window.draw(headingText);
                window.draw(startingText);
                window.display(); 
            }      
                // The actual game
                // logic
                Clock clock;
                if (Keyboard::isKeyPressed(Keyboard::Space)) {
                    mainMenuSound.pause();
                    gameSound.play();
                    gameSound.setLoop(true);
                    while (numberOfTargets < 2)
                    {
                        window.clear();
                        Time timeElasped = clock.getElapsedTime();
                        // Score gets deducted because player is taking time to get targets!
                        if (timeElasped.asSeconds() > 0.5f) {
                            clock.restart();
                            score--;
                        }

                        scoreText.setString("Score: " + to_string(score));
                        // Drawing everything
                        window.draw(scoreText);
                        window.draw(leftWall);
                        window.draw(rightWall);
                        window.draw(topWall);
                        window.draw(bottomWall);

                        Update(&world);
                        applyForces(snakeBody);
                        display(target, snakeBody);

                        // set position. Have to transfer the coordinate system because its different in SFML and Box2D
                        snakeObject.setPosition(snakeBody->GetPosition().x + 300, 300 - (snakeBody->GetPosition().y));
                        window.draw(targetObject);
                        window.draw(snakeObject);
                        window.display();

                        // Checking for Target
                        if (GetDistance(snakeBody->GetWorldCenter(), *target) < 13.0f) {
                            score += 1000;
                            numberOfTargets++;
                            moveTarget(target->x, target->y);

                            cout << "New Target Acquired!" << endl;
                        }
                        targetObject.setPosition(300 + target->x, 300 - target->y);
                    }
                    window.clear();
                    startingText.setString("Congratulations! You Win!\n Final Score: " + to_string(score));
                    startScreen = false;
                    gameSound.pause();
                    mainMenuSound.play();
                    window.draw(startingText);
                    window.display();
                   
                }  
        } 
    }
}

// function to get distance
float GetDistance(b2Vec2 pos1, b2Vec2 pos2) {

    return sqrtf(powf(pos1.x - pos2.x, 2)+ powf(pos1.y - pos2.y, 2));
}






