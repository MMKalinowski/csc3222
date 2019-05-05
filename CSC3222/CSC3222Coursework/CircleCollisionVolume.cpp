#pragma once
#include "CircleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CircleCollisionVolume::CircleCollisionVolume(Vector2 position, Vector2 offset, int radius) : CollisionVolume()
{
	this->radius = radius;
	this->position = position + offset;
	this->type = 2;
}
CircleCollisionVolume::~CircleCollisionVolume()
{

}