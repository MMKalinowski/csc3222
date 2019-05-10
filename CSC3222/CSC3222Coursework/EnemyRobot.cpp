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

	this->inverseMass = 1;

	direction = {};

	currentState = State::Idle;
}

EnemyRobot::~EnemyRobot()
{}

bool EnemyRobot::UpdateObject(float dt)
{
	thinkTime -= dt;

	if (this->player)
	{
		FSM(dt, this->player);
	}

	if (moving)
	{
		this->AddForce(direction * this->testSpeed);
		UpdateAnimFrame(dt);
	}

	if (thinkTime < 0 && currentState == State::Idle)
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
	else if (thinkTime < 0 && currentState == State::Attack)
	{
		moving = false;
		thinkTime += 0.5f;

		Vector2 direction = Vector2(-testSpeed, 0);
		currentAnimDir = MovementDir::Left;

		//further attack logic here
	}

	this->collider->updatePos(this->position + this->collider->getOffset());

	return true;
}

void EnemyRobot::FSM(float dt, CollisionVolume* player)
{
	switch (currentState)
	{
		case State::Idle:
			if ((this->collider->getPosition() - player->getPosition()).Length() < 80)
			{
				currentState = State::Attack;
				std::cout << "attack activated" << std::endl;
			}
			//if exists bonus robot, go to defend
			break;
		case State::Attack:
			if ((this->collider->getPosition() - player->getPosition()).Length() >= 80)
			{
				currentState = State::Idle;
				std::cout << "idle activated" << std::endl;
			}
			//if exists bonus robot, go to defend
			break;
		case State::Defend:
			//if not exists bonus robot, go to idle
			break;
		default:
			break;
	}
}

void EnemyRobot::addPlayer(CollisionVolume* player)
{
	this->player = player;
}