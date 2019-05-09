#pragma once
#include "CollisionVolume.h"
//#include "RigidBody.h"
#include "../../Common/Vector2.h"

namespace NCL
{
	using namespace Maths;
	namespace CSC3222
	{
		class RectangleCollisionVolume : public CollisionVolume
		{
			public:
			RectangleCollisionVolume(Vector2 position, Vector2 offset, int x_size, int y_size, RigidBody* = nullptr, ColliderTag = ColliderTag::Free);
			~RectangleCollisionVolume();

			int getX_Size() { return x_size; }
			int getY_Size() { return y_size; }

			Vector2 getPosition() override { return position; }

			private:
			int x_size;
			int y_size;
		};
	}
}