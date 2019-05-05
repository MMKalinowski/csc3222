#pragma once
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"

namespace NCL
{
	using namespace Maths;
	namespace CSC3222
	{
		class RectangleCollisionVolume : public CollisionVolume
		{
			public:
			RectangleCollisionVolume(Vector2 offset, int x_size, int y_size);
			~RectangleCollisionVolume();

			private:
			int x_size;
			int y_size;
			Vector2 offset;
		};
	}
}