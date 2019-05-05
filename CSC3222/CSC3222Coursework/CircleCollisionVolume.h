#pragma once
#include "CollisionVolume.h"
namespace NCL
{
	namespace CSC3222
	{
		class CircleCollisionVolume : public CollisionVolume
		{
			public:
			CircleCollisionVolume(int offset, int radius);
			~CircleCollisionVolume();

			private:
			int radius;
			int offset;
		};
	}
}