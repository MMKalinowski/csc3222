#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume(Maths::Vector2 position, Maths::Vector2 offset, int type)
{
	this->offset = offset;
	this->type = type;
	this->position = position + offset;
}


CollisionVolume::~CollisionVolume()
{}
