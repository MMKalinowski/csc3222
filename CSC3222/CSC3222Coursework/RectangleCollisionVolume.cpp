#pragma once
#include "RectangleCollisionVolume.h"


using namespace NCL;
using namespace CSC3222;

RectangleCollisionVolume::RectangleCollisionVolume(Vector2 position, Vector2 offset, int x, int y, RigidBody* rb) 
: CollisionVolume(position, offset, 1, rb)
{
	this->x_size = x;
	this->y_size = y; //initialiser list
}
RectangleCollisionVolume::~RectangleCollisionVolume()
{

}