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
	for (unsigned int i = 0; i < allColliders.size(); ++i)
	{
		for (unsigned int j = i + 1; j < allColliders.size(); ++j)
		{
			CheckCollision(allColliders[i], allColliders[j]);
		}
	}
}

bool GameSimsPhysics::CheckCollision(CollisionVolume* l, CollisionVolume* r)
{
	if (l->getType() == r->getType())
	{
		if (l->getType() == 2)
		{
			CircleCollisionVolume* left = dynamic_cast<CircleCollisionVolume*>(l);
			CircleCollisionVolume* right = dynamic_cast<CircleCollisionVolume*>(r);
			return CircleCircle(left, right);
		}
		else
		{
			RectangleCollisionVolume* left = dynamic_cast<RectangleCollisionVolume*>(l);
			RectangleCollisionVolume* right = dynamic_cast<RectangleCollisionVolume*>(r);
			return RectRect(left, right);
		}
	}
	else
	{
		return CircleRect(l, r);
	}

	return false;
}

bool GameSimsPhysics::CircleCircle(CircleCollisionVolume* l, CircleCollisionVolume* r)
{
	int radiusL = l->getRadius();
	int radiusR = r->getRadius();
	Vector2 posL = l->getPosition();
	Vector2 posR = r->getPosition();

	if (pow((posR.x - posL.x), 2) + pow((posL.y - posR.y), 2) <= pow((radiusL + radiusR), 2))
		return true;

	return false;
}

bool GameSimsPhysics::RectRect(RectangleCollisionVolume* l, RectangleCollisionVolume* r)
{
	int xdelta = abs(l->getPosition().x - r->getPosition().x);
	int ydelta = abs(l->getPosition().y - r->getPosition().y);

	if (xdelta < ((l->getX_Size() / 2) + (r->getX_Size() / 2)) && ydelta < ((l->getY_Size() / 2) + (r->getY_Size() / 2)))
	{
		std::cout << "COLLIDE PLOX" << std::endl;
		return true;
	}

	return false;

	/*Vector2 l1 = l->getPosition();
	l1.x = l1.x - (l->getX_Size() / 2);
	l1.y = l1.y - (l->getY_Size() / 2);

	Vector2 r1 = l1;
	r1.x = r1.x + l->getX_Size();
	r1.y = r1.y + l->getY_Size();

	Vector2 l2 = r->getPosition();
	l2.x = l2.x - (r->getX_Size() / 2);
	l2.y = l2.y - (r->getY_Size() / 2);

	Vector2 r2 = l2;
	r2.x = r2.x + r->getX_Size();
	r2.y = r2.y + r->getY_Size();

	if ((l1.x < r2.x || l2.x < r1.x))
	{
		if (r1.y > l2.y || l1.y < r2.y)
		{
			std::cout << "COLLIDE PLOX" << std::endl;
			std::cout << "Rect1: l1 " << l1 << "; r1 " << r1 << std::endl;
			std::cout << "Rect2: l2 " << l2 << "; r2 " << r2 << std::endl;
			return true;
		}
	}

	return false;*/
}
bool GameSimsPhysics::CircleRect(CollisionVolume* l, CollisionVolume* r)
{
	return false;
}