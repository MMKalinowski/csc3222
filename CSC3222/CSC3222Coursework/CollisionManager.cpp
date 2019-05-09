#include "CollisionManager.h"
using namespace NCL::CSC3222;

static CollisionVolume* isEither(CollisionVolume* l, CollisionVolume* r, ColliderTag t)
{
	if (l->getTag() == t)
	{
		return l;
	}
	if (r->getTag() == t)
	{
		return r;
	}
	
	return nullptr;
}

bool CollisionManager::shouldCollide(CollisionVolume* l, CollisionVolume* r, Collision col)
{
	ColliderTag tagLeft = l->getTag();
	ColliderTag tagRight = r->getTag();
	
	if (l->getTag() == r->getTag())
	{
		return !(tagLeft == ColliderTag::Bullet || tagLeft == ColliderTag::Collectible);
	}

	if (isEither(l, r, ColliderTag::Bullet))
	{
		CollisionVolume* enemy = isEither(l, r, ColliderTag::Enemy);
		if (enemy)
		{
			enemy->getRigidBody();
		}
	}

	//anything red -> blue exit
	//anything blue -> green exit
	//anything green -> red exit
	//collectible player -> picked up collectible
	//player home -> if has collectible, delete collectible, player collides
	//player (bullet || enemy) -> delete bullet, stun enemy, player loses life

	return true;
}