#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"

namespace NCL
{
	using namespace Maths;
	namespace CSC3222
	{
		class CircleCollisionVolume : public CollisionVolume
		{
			public:
			CircleCollisionVolume(Vector2 position, Vector2 offset, int radius);
			~CircleCollisionVolume();

			int getRadius() { return radius; }
			Vector2 getPosition() { return position; }

			private:
			int radius;
			Vector2 position;
		};
	}
}