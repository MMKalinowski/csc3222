#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume(Maths::Vector2 position, Maths::Vector2 offset, int type, RigidBody* rb, ColliderTag tag)
: offset(offset)
, type(type)
, position(position + offset)
, rb(rb)
, tag(tag)
{
	if (rb)
	{
		rb->SetCollider(this);
	}
}


CollisionVolume::~CollisionVolume()
{}
