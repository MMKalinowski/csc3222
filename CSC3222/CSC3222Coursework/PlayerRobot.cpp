#include "PlayerRobot.h"
#include "Laser.h"
#include "TextureManager.h"
#include "RobotRescueGame.h"
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"

#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

PlayerRobot::PlayerRobot() : Robot()
{
	texture = texManager->GetTexture("Turret Bot.png");

	position = Vector2(32, 64);

	this->inverseMass = 1;
	//this->SetCollider(new CircleCollisionVolume(position, Vector2(8,24), 7));
	SetCollider(new RectangleCollisionVolume(position, Vector2(8, 24), 16, 16));
	this->collider->addTag(ColliderTag::Player);
}

PlayerRobot::~PlayerRobot()
{

}

bool PlayerRobot::UpdateObject(float dt)
{
	float testSpeed = 80;

	Vector2 laserDir;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
	{
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		laserDir.x = -testSpeed * dt;
		/*this->AddForce(Vector2{-testAccel/this->inverseMass, 0});*/
		
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
	{
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		laserDir.x = testSpeed * dt;
		/*this->AddForce(Vector2{ testAccel / this->inverseMass, 0 });*/
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
	{
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);

		laserDir.y = -testSpeed * dt;
		/*this->AddForce(Vector2{ 0, -testAccel / this->inverseMass });*/
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
	{
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		laserDir.y = testSpeed * dt;
		/*this->AddForce(Vector2{ 0, testAccel / this->inverseMass });*/
	}

	this->AddForce(laserDir * testSpeed);

	//TODO: hotfix bug when pressing Ctrl fast a one-frame jerk of already existing lasers
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_CONTROL))
	{
		laserDir.Normalize();
		Laser* shot = new Laser(laserDir);
		shot->SetPosition(position + Vector2(8, 16));

		game->AddNewObject(shot);
		laserDir.ToZero();
	}

	this->collider->updatePos(this->position + this->collider->getOffset());

	return true;
}