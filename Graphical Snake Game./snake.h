#pragma once
#include <Box2D/Box2D.h>
void Wrapper();
void Update(b2World* world);
void applyForces(b2Body* snake);
void moveTarget(float& xPos, float& yPos);
void display(b2Vec2* target, b2Body* snake);
void InitializeWallsObjects(b2World* world);
float GetDistance(b2Vec2 pos1, b2Vec2 pos2);