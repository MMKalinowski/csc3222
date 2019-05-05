#pragma once
#include "CircleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CircleCollisionVolume::CircleCollisionVolume(int offset, int radius) : CollisionVolume()
{
	this->radius = radius;
	this->offset = offset;
}
CircleCollisionVolume::~CircleCollisionVolume()
{

}