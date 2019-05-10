#include "Robot.h"
#include "GameSimsRenderer.h"
#include "CollisionVolume.h"
#include "RectangleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;
using namespace Maths;

int  startY[] = { 0,32,64,64 };
int  startX[] = { 0,16,32,48 };
bool flips[] = { false,false,false,true };

Robot::Robot() : SimObject()
{
	currentAnimDir = Left;
	currentanimFrame = 0;
}

Robot::~Robot()
{

}

void Robot::DrawObject(GameSimsRenderer &r)
{
	//determine frame of anim to play, and whether its flipped or not

	Vector2 screenPos = position;

	Vector2 texPos;

	Vector2 texSize = Vector2(16, 32);

	texPos.x = float(startX[currentanimFrame]);
	texPos.y = float(startY[currentAnimDir]);

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, flips[currentAnimDir]);
	
	/*if(this->collider->getType() == 2)
		r.DrawCircle(this->collider->getPosition(), 7);
	else
	{
		RectangleCollisionVolume* rcv = reinterpret_cast<RectangleCollisionVolume*>(this->collider);
		r.DrawBox(
			this->collider->getPosition(),
			Vector2(rcv->getX_Size() / 2, rcv->getY_Size() / 2));
	}*/
}

void Robot::UpdateAnimFrame(float dt)
{
	frameTime -= dt;

	if (frameTime < 0)
	{
		currentanimFrame = (currentanimFrame + 1) % 4;
		frameTime += 1.0f / 12.0f;
	}
}
