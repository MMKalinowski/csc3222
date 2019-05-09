#include "Laser.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "CircleCollisionVolume.h"

#include "../../Common/Maths.h"

#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

using namespace NCL;
using namespace CSC3222;

Vector4 frames[16] = {
	Vector4(0, 36, 16 , 7), //9 o clock

	Vector4(1, 21, 16, 8),
	Vector4(3, 4, 13, 12),
	Vector4(22, 1, 8 ,15),
	Vector4(37, 1, 7 ,16), //12 0 clock

	Vector4(51,1, 8, 15),
	Vector4(65, 4 , 13 ,12),
	Vector4(65, 21, 15, 8),
	Vector4(65, 36, 16, 7),//3 o clock

	Vector4(65, 50, 15 , 8),
	Vector4(65, 63, 13, 12),
	Vector4(51, 63, 8, 15),
	Vector4(37, 63, 7 ,16), //6 o clock
	Vector4(22, 63 , 8, 15),
	Vector4(3, 63, 13 ,12),
	Vector4(1, 50, 15, 8)
};

Laser::Laser(Vector2& direction) : SimObject()
{
	texture = texManager->GetTexture("bullet.png");
	
	velocity = (direction.x != 0 || direction.y != 0) ? direction : Vector2{ 1.0f, .0f };
	velocity *= 100;
	dir = velocity;

	this->SetCollider(new CircleCollisionVolume(position, { 0, 0 }, 3, this));

	//inverseMass = 1.f / float(rand() % 5 + 1); //randomisation for physics test
	inverseMass = 100;
	this->AddForce(dir * SPEED / inverseMass);
}

Laser::~Laser()
{}

void Laser::DrawObject(GameSimsRenderer &r)
{
	Vector2 screenPos = position;
	Vector2 texPos;
	Vector2 texSize = Vector2(16, 16);

	const double test = atan2(velocity.y, velocity.x);
	const double degrees = std::fmodf(Maths::RadiansToDegrees(test) + 180.0f, 360.f);

	const int frame = degrees * 16 / 360;

	texPos.x = frames[frame].x;
	texPos.y = frames[frame].y;

	texSize.x = frames[frame].z;
	texSize.y = frames[frame].w;

	this->GetCollider()->setOffset(texSize / 2.f);

	r.DrawTextureArea(reinterpret_cast<OGLTexture*>(texture), texPos, texSize, screenPos);
	r.DrawCircle(this->GetCollider()->getPosition(), 3);
}

bool Laser::UpdateObject(const float dt)
{
	/*Vector2 currentVel = this->GetVelocity();
	currentVel = (currentVel.x != 0 || currentVel.y != 0) ? currentVel : Vector2{1.0f, .0f};
	
	Vector2 nextPosition = this->GetPosition() + (currentVel * SPEED * dt);
	this->SetPosition(nextPosition);*/

	float speed = this->GetVelocity().Length();
	if (speed != SPEED)
	{
		Vector2 newVel = this->GetVelocity().Normalize();
		this->SetVelocity(newVel * speed);
	}

	return position.x >= 0 && position.y >= 0 && position.x < 30 * 16 && position.y < 20 * 16;
	//return true;
}