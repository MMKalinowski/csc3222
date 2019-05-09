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
		obj->SetVelocity(obj->GetVelocity() + (accel * dt * 0.999f));
	}
}

void GameSimsPhysics::IntegrateVel(const float dt)
{
	for (auto obj : allBodies)
	{
		obj->SetPosition(obj->GetPosition() + obj->GetVelocity() * dt);
		obj->force.ToZero();
		if (obj->GetCollider())
		{
			obj->GetCollider()->updatePos(obj->GetPosition() + obj->GetCollider()->getOffset());
		}
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
				}
			}
		}
	}
}

void GameSimsPhysics::ResolveCollision(CollisionVolume* l, CollisionVolume* r, Collision col)
{
	RigidBody* const lBody = l->getRigidBody();
	RigidBody* const rBody = r->getRigidBody();

	if (!lBody && !rBody)
	{
		return;
	}
	if (!lBody || !rBody)
	{
		return;
	}

	const auto left  = std::make_pair(lBody->GetPosition(), lBody->GetMass());
	const auto right = std::make_pair(rBody->GetPosition(), rBody->GetMass());

	const float totalMass = left.second + right.second;
	
	if (totalMass)
	{
		const Vector2 lPos = left.first;
		const Vector2 rPos = right.first;

		Vector2 newPos = lPos + (col.normal * col.penetration * (left.second / totalMass));
		lBody->SetPosition(newPos);
		l->updatePos(newPos + l->getOffset());
		
		newPos = rPos - (col.normal * col.penetration * (right.second / totalMass));
		rBody->SetPosition(newPos);
		r->updatePos(newPos + r->getOffset());

		//elasticity
		float e = .9f;

		const Vector2 relativeVel = rBody->GetVelocity() - lBody->GetVelocity();
		float impulse = (-1*(1 + e) * (relativeVel.Dot(col.normal))) / totalMass;

		lBody->SetVelocity(lBody->GetVelocity() - (col.normal * lBody->GetMass() * impulse));
		rBody->SetVelocity(rBody->GetVelocity() + (col.normal * rBody->GetMass() * impulse));
	}
}

Collision GameSimsPhysics::CheckCollision(CollisionVolume* l, CollisionVolume* r)
{
	if (l->getType() == r->getType())
	{
		if (l->getType() == 2)
		{
			CircleCollisionVolume* left  = reinterpret_cast<CircleCollisionVolume*>(l);
			CircleCollisionVolume* right = reinterpret_cast<CircleCollisionVolume*>(r);
			return CircleCircle(left, right);
		}
		else
		{
			RectangleCollisionVolume* left  = reinterpret_cast<RectangleCollisionVolume*>(l);
			RectangleCollisionVolume* right = reinterpret_cast<RectangleCollisionVolume*>(r);
			return RectRect(left, right);
		}
	}
	return CircleRect(l, r);
}

Collision GameSimsPhysics::CircleCircle(CircleCollisionVolume* l, CircleCollisionVolume* r)
{
	float radiusL = l->getRadius();
	float radiusR = r->getRadius();
	Vector2 posL = l->getPosition();
	Vector2 posR = r->getPosition();
	
	return { (posL - posR).Normalize(), ((radiusL + radiusR) - (posL - posR).Length()) };
}

Collision GameSimsPhysics::RectRect(RectangleCollisionVolume* l, RectangleCollisionVolume* r)
{
	//these 2 are bloat
	const Vector2 lpos = l->getPosition();
	const Vector2 rpos = r->getPosition();

	const Vector2 lsiz = { float(l->getX_Size()), float(l->getY_Size()) };
	const Vector2 rsiz = { float(r->getX_Size()), float(r->getY_Size()) };

	const float lMinX = lpos.x - lsiz.x / 2;
	const float lMaxX = lpos.x + lsiz.x / 2;
	const float lMinY = lpos.y - lsiz.y / 2;
	const float lMaxY = lpos.y + lsiz.y / 2;

	const float rMinX = rpos.x - rsiz.x / 2;
	const float rMaxX = rpos.x + rsiz.x / 2;
	const float rMinY = rpos.y - rsiz.y / 2;
	const float rMaxY = rpos.y + rsiz.y / 2;

	const float il = std::max(lMinX, rMinX);
	const float it = std::max(lMinY, rMinY);
	const float ir = std::min(lMaxX, rMaxX);
	const float ib = std::min(lMaxY, rMaxY);

	return { (lpos - rpos).Normalize(), std::min(ir - il, ib - it) };
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

	const float xmin = rectpos.x - (right->getX_Size() / 2);
	const float xmax = rectpos.x + (right->getX_Size() / 2);
	const float ymin = rectpos.y - (right->getY_Size() / 2);
	const float ymax = rectpos.y + (right->getY_Size() / 2);

	circpos.x = Maths::Clamp(circpos.x, xmin, xmax);
	circpos.y = Maths::Clamp(circpos.y, ymin, ymax);

	const float dist = pow(left->getPosition().x - circpos.x, 2) + pow(left->getPosition().y - circpos.y, 2);

	return { (left->getPosition() - circpos).Normalize(), float(left->getRadius() - std::sqrt(dist)) };
}
