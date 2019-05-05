#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
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
	for (int i = 0; i < allColliders.size(); ++i)
	{
		for (int j = i + 1; i < allColliders.size(); ++j)
		{
			CheckCollision(allColliders[i], allColliders[j]);
		}
	}
}

bool GameSimsPhysics::CheckCollision(CollisionVolume* l, CollisionVolume* r)
{
	return false;
}
