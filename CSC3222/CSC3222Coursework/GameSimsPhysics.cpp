#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"
#include "../../Common/Vector2.h"

using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics()
{

}

GameSimsPhysics::~GameSimsPhysics()
{

}

void GameSimsPhysics::Update(const float dt)
{
	timeUntilUpdate += dt;
	
	while (timeUntilUpdate > subDT)
	{
		IntegrateAccel(dt);
		CollisionDetection(dt);
		IntegrateVel(dt);
		
		timeUntilUpdate -= subDT;
		//std::cout << allColliders.size() << std::endl;
	}
	
}

void GameSimsPhysics::AddRigidBody(RigidBody* b)
{
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b)
{
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end())
	{
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c)
{
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c)
{
	auto at = std::find(allColliders.begin(), allColliders.end(), c);

	if (at != allColliders.end())
	{
		//maybe delete too?
		allColliders.erase(at);
	}
}

void GameSimsPhysics::IntegrateAccel(const float dt)
{
	for (auto obj : allBodies)
	{
		Vector2 accel = obj->force * obj->inverseMass;
		obj->SetVelocity(accel + (accel * dt * 0.999f));
	}
}

void GameSimsPhysics::IntegrateVel(const float dt)
{
	for (auto obj : allBodies)
	{
		obj->SetPosition(obj->GetPosition() + obj->GetVelocity() * dt);
		obj->force.ToZero();
	}
}

void GameSimsPhysics::CollisionDetection(const float dt)
{
	for (CollisionVolume *c : allColliders)
	{
		for (CollisionVolume *c2 : allColliders)
		{
			if (c != c2)
			{
				const auto collision = CheckCollision(c, c2);
				if (collision.occured())
				{
					ResolveCollision(c, c2, collision);
					//std::cout << "golizja :~~DD" << std::endl;
				}
			}
		}
	}
}

void GameSimsPhysics::ResolveCollision(CollisionVolume* l, CollisionVolume* r, Collision col)
{

}

Collision GameSimsPhysics::CheckCollision(CollisionVolume* l, CollisionVolume* r)
{
	if (l->getType() == r->getType())
	{
		if (l->getType() == 2)
		{
			CircleCollisionVolume* left = reinterpret_cast<CircleCollisionVolume*>(l);
			CircleCollisionVolume* right = reinterpret_cast<CircleCollisionVolume*>(r);
			return CircleCircle(left, right);
		}
		else
		{
			RectangleCollisionVolume* left = reinterpret_cast<RectangleCollisionVolume*>(l);
			RectangleCollisionVolume* right = reinterpret_cast<RectangleCollisionVolume*>(r);
			return RectRect(left, right);
		}
	}
	else
	{
		return CircleRect(l, r);
		//return false;
	}
	//return false;
}

Collision GameSimsPhysics::CircleCircle(CircleCollisionVolume* l, CircleCollisionVolume* r)
{
	int radiusL = l->getRadius();
	int radiusR = r->getRadius();
	Vector2 posL = l->getPosition();
	Vector2 posR = r->getPosition();
	
	return { (posL - posR).Normalize(), int((radiusL + radiusR) - (posL - posR).Length()) };

	//return (pow((posR.x - posL.x), 2) + pow((posL.y - posR.y), 2) <= pow((radiusL + radiusR), 2));
		//return true;//return {};

	//return false;
}

Collision GameSimsPhysics::RectRect(RectangleCollisionVolume* l, RectangleCollisionVolume* r)
{
	////these 2 are bloat
	//int xdelta = abs(l->getPosition().x - r->getPosition().x);
	//int ydelta = abs(l->getPosition().y - r->getPosition().y);

	const Vector2 lpos = l->getPosition();
	const Vector2 rpos = r->getPosition();

	const Vector2 lsiz = { float(l->getX_Size()), float(l->getY_Size()) };
	const Vector2 rsiz = { float(r->getX_Size()), float(r->getY_Size()) };

	const int lMinX = min(lpos.x - lsiz.x / 2, lpos.x + lsiz.x / 2);
	const int lMaxX = max(lpos.x - lsiz.x / 2, lpos.x + lsiz.x / 2);
	const int lMinY = min(lpos.y - lsiz.y / 2, lpos.y + lsiz.y / 2);
	const int lMaxY = max(lpos.y - lsiz.y / 2, lpos.y + lsiz.y / 2);

	const int rMinX = min(rpos.x - rsiz.x / 2, rpos.x + rsiz.x / 2);
	const int rMaxX = max(rpos.x - rsiz.x / 2, rpos.x + rsiz.x / 2);
	const int rMinY = min(rpos.y - rsiz.y / 2, rpos.y + rsiz.y / 2);
	const int rMaxY = max(rpos.y - rsiz.y / 2, rpos.y + rsiz.y / 2);

	const int il = max(lMinX, rMinX);
	const int it = max(lMinY, rMinY);
	const int ir = min(lMaxX, rMaxX);
	const int ib = min(lMaxY, rMaxY);

	return { (lpos - rpos).Normalize(), int((Vector2(il, it) - Vector2(ir, ib)).Length()) };

	//return (xdelta < (l->getX_Size() + r->getX_Size()) / 2 && ydelta < (l->getY_Size() + r->getY_Size()) / 2);
}

Collision GameSimsPhysics::CircleRect(CollisionVolume* l, CollisionVolume* r)
{
	CircleCollisionVolume* left;
	RectangleCollisionVolume* right;

	if (l->getType() == 2)
	{
		left = reinterpret_cast<CircleCollisionVolume*>(l);
		right = reinterpret_cast<RectangleCollisionVolume*>(r);
	}
	else
	{
		left = reinterpret_cast<CircleCollisionVolume*>(r);
		right = reinterpret_cast<RectangleCollisionVolume*>(l);
	}

	Vector2 circpos = left->getPosition();
	Vector2 rectpos = right->getPosition();

	const int xmin = rectpos.x - (right->getX_Size() / 2);
	const int xmax = rectpos.x + (right->getX_Size() / 2);
	const int ymin = rectpos.y - (right->getY_Size() / 2);
	const int ymax = rectpos.y + (right->getY_Size() / 2);

	circpos.x = clamp(circpos.x, xmin, xmax);
	circpos.y = clamp(circpos.y, ymin, ymax);

	const int dist = pow(left->getPosition().x - circpos.x, 2) + pow(left->getPosition().y - circpos.y, 2);

	return { (left->getPosition() - circpos).Normalize(), int(left->getRadius() - std::sqrt(dist)) };

	//return dist < pow(left->getRadius(), 2);
}
