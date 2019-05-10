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

	if (isEither(l, r, ColliderTag::Slowdown))
	{
		//slow player || enemy by 30%
		return false;
	}

	CollisionVolume* red = isEither(l, r, ColliderTag::Red);

	if (red)
	{
		CollisionVolume* notRed = l == red ? r : l;
		Vector2 tepPos = map->teleportTo(MapStructureType::BlueTeleporter);
		RigidBody* rb =  notRed->getRigidBody();

		if (rb)
		{
			rb->SetPosition(tepPos);
		}
		notRed->updatePos(tepPos + notRed->getOffset());
		return false;
	}

	CollisionVolume* blue = isEither(l, r, ColliderTag::Blue);

	if (blue)
	{
		CollisionVolume* notBlue = l == blue ? r : l;
		Vector2 tepPos = map->teleportTo(MapStructureType::GreenTeleporter);
		RigidBody* rb = notBlue->getRigidBody();

		if (rb)
		{
			rb->SetPosition(tepPos);
		}
		notBlue->updatePos(tepPos + notBlue->getOffset());
		return false;
	}

	CollisionVolume* green = isEither(l, r, ColliderTag::Green);

	if (green)
	{
		CollisionVolume* notGreen = l == green ? r : l;
		Vector2 tepPos = map->teleportTo(MapStructureType::RedTeleporter);
		RigidBody* rb = notGreen->getRigidBody();

		if (rb)
		{
			rb->SetPosition(tepPos);
		}
		notGreen->updatePos(tepPos + notGreen->getOffset());
		return false;
	}

	/*if (isEither(l, r, ColliderTag::Collectible))
	{
		CollisionVolume* enemy = isEither(l, r, ColliderTag::Enemy);
		if (enemy)
		{
			enemy->getRigidBody();
			return false;
		}

		CollisionVolume* bullet = isEither(l, r, ColliderTag::Bullet);
		if (bullet)
		{
			bullet->getRigidBody();
			return false;
		}

		return true;
	}*/


	//collectible player -> picked up collectible
	//player home -> if has collectible, delete collectible, player collides
	//player (bullet || enemy) -> delete bullet, stun enemy, player loses life

	return true;
}