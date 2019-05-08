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
	this->SetCollider(new CircleCollisionVolume(position, Vector2(8,24), 7));
	//this->SetCollider(new RectangleCollisionVolume(position, Vector2(8, 24), 16, 16));
	this->collider->setTag(ColliderTag::Player);
}

PlayerRobot::~PlayerRobot()
{

}

bool PlayerRobot::UpdateObject(float dt)
{
	float testSpeed = 80;

	Vector2 laserDir;

	this->SetVelocity({ 0, 0 });

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
	{
		currentAnimDir = MovementDir::Left;
		UpdateAnimFrame(dt);

		laserDir.x = -1;
		this->SetVelocity(this->GetVelocity() + Vector2{-testSpeed, 0});
		/*this->AddForce(Vector2{-testAccel/this->inverseMass, 0});*/
		
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
	{
		currentAnimDir = MovementDir::Right;
		UpdateAnimFrame(dt);

		laserDir.x = 1;
		this->SetVelocity(this->GetVelocity() + Vector2{ testSpeed, 0 });
		/*this->AddForce(Vector2{ testAccel / this->inverseMass, 0 });*/
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
	{
		currentAnimDir = MovementDir::Up;
		UpdateAnimFrame(dt);

		laserDir.y = -1;
		this->SetVelocity(this->GetVelocity() + Vector2{ 0, -testSpeed });
		/*this->AddForce(Vector2{ 0, -testAccel / this->inverseMass });*/
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
	{
		currentAnimDir = MovementDir::Down;
		UpdateAnimFrame(dt);

		laserDir.y = 1;
		this->SetVelocity(this->GetVelocity() + Vector2{ 0, testSpeed });
		/*this->AddForce(Vector2{ 0, testAccel / this->inverseMass });*/
	}

	//this->AddForce(laserDir * testSpeed);
	this->collider->updatePos(this->position + this->collider->getOffset());

	//TODO: hotfix bug when pressing Ctrl fast a one-frame jerk of already existing lasers
	if (Window::GetKeyboard()->KeyPressed(KEYBOARD_CONTROL))
	{
		laserDir = laserDir.Normalize();
		Laser* shot = new Laser(laserDir);
		shot->SetPosition(this->collider->getPosition() + laserDir * 16.f);

		game->AddNewObject(shot);
		laserDir.ToZero();
	}


	return true;
}