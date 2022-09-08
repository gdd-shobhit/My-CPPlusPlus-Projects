#define SFML_STATIC
#include "snake.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>
#include <conio.h>
#include <cmath>
#include <iomanip>

using namespace std;

void Update(b2World* world) {

    float timeStep = 1.0f / 100.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    
    world->Step(timeStep, velocityIterations, positionIterations);       
}

void InitializeWallsObjects(b2World* world) {
    // Creating Walls
    
    // left wall
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.type = b2_staticBody;
    leftWallBodyDef.position.Set(-290.0f, 0.0f);
    b2Body* leftWallBody = world->CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f,300.0f);
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    // right wall
    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.type = b2_staticBody;
    rightWallBodyDef.position.Set(295.0f, 0.0f);
    b2Body* rightWallBody = world->CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(20.0f, 300.0f);
    rightWallBody->CreateFixture(&rightWallBox, 0.0f);
    cout << "rightWall pos: " << rightWallBody->GetPosition().x << endl;

    // bottom wall
    b2BodyDef bottomWallBodyDef;
    bottomWallBodyDef.type = b2_staticBody;
    bottomWallBodyDef.position.Set(0.0f, -270.0f);
    b2Body* bottomWallBody = world->CreateBody(&bottomWallBodyDef);
    b2PolygonShape bottomWallBox;
    bottomWallBox.SetAsBox(300.0f, 20.0f);
    bottomWallBody->CreateFixture(&bottomWallBox, 0.0f);

    // top wall
    b2BodyDef topWallBodyDef;
    topWallBodyDef.type = b2_staticBody;
    topWallBodyDef.position.Set(0.0f, 300.0f);
    b2Body* topWallBody = world->CreateBody(&topWallBodyDef);
    b2PolygonShape topWallBox;
    topWallBox.SetAsBox(300.0f,20.0f);
    topWallBody->CreateFixture(&topWallBox, 0.0f);
}

// displays the position
void display(b2Vec2* target, b2Body* snake) {
	// not required now
}

//applies the force
void applyForces(b2Body* snake) {
    b2Vec2* upwardForce = new b2Vec2(0.0f, 50.0f);
    b2Vec2* leftwardForce = new b2Vec2(-40.0f, 0.0f);
    b2Vec2* rightwardForce = new b2Vec2(40.0f, 0.0f);

    // Movement
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {        
           snake->ApplyForceToCenter(*upwardForce,true);
            }         
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
           snake->ApplyForceToCenter(*rightwardForce, true);
            }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
           snake->ApplyForceToCenter(*leftwardForce, true);
            }
    
    delete upwardForce;
    delete leftwardForce;
    delete rightwardForce;
    upwardForce = nullptr;
    leftwardForce = nullptr;
    rightwardForce = nullptr;
}

// shifts the targets
void moveTarget(float& xPos, float& yPos) {
    float LO = -5.0f;
    float HI = 5.0f;
    float randomNumber1 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
    float randomNumber2 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
    xPos = (randomNumber1);
    yPos = (randomNumber2);
}