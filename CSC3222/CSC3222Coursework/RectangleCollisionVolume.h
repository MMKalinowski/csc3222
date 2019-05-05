#pragma once
#include "CollisionVolume.h"
namespace NCL
{
	namespace CSC3222
	{
		class RectangleCollisionVolume : public CollisionVolume
		{
			public:
			RectangleCollisionVolume(int offset, int x_size, int y_size);
			~RectangleCollisionVolume();

			private:
			int x_size;
			int y_size;
			int offset;
		};
	}
}