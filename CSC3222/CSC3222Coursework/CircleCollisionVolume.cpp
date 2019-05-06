#pragma once
#include "CircleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CircleCollisionVolume::CircleCollisionVolume(Vector2 position, Vector2 offset, int radius)
: CollisionVolume(position, offset, 2)
{
	this->radius = radius;
}
CircleCollisionVolume::~CircleCollisionVolume()
{

}