#include "EnemyRobot.h"
#include "TextureManager.h"
#include "CollisionVolume.h"
#include "../../Common/TextureLoader.h"
using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

EnemyRobot::EnemyRobot() : Robot()
{
	texture = texManager->GetTexture("Turret Bot overgrown.png");

	moving = false;

	this->inverseMass = 1.5;

	direction = Vector2();
}

EnemyRobot::~EnemyRobot()
{}

bool EnemyRobot::UpdateObject(float dt)
{
	thinkTime -= dt;

	if (moving)
	{
		//position += direction * dt;
		this->AddForce(direction * this->testSpeed);
		UpdateAnimFrame(dt);
	}

	if (thinkTime < 0)
	{
		moving = false;
		direction = Vector2();

		thinkTime += 0.5f;

		int choice = rand() % 5;

		if (choice == 1)
		{
			moving = true;
			direction.x = -testSpeed;
			currentAnimDir = MovementDir::Left;
		}

		if (choice == 2)
		{
			moving = true;
			direction.x = testSpeed;
			currentAnimDir = MovementDir::Right;
		}

		if (choice == 3)
		{
			moving = true;
			direction.y = -testSpeed;
			currentAnimDir = MovementDir::Up;
		}

		if (choice == 4)
		{
			moving = true;
			direction.y = testSpeed;
			currentAnimDir = MovementDir::Down;
		}
	}

	this->collider->updatePos(this->position + this->collider->getOffset());

	return true;
}