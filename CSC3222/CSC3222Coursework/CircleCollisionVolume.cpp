#pragma once
#include "CircleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CircleCollisionVolume::CircleCollisionVolume(Vector2 position, Vector2 offset, int radius, RigidBody* rb, ColliderTag tag)
: CollisionVolume(position, offset, 2, rb, tag)
{
	this->radius = radius; //initialiser list
}
CircleCollisionVolume::~CircleCollisionVolume()
{}