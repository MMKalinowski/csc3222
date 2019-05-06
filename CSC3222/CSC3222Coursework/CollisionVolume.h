#pragma once
#include "../../Common/Vector2.h"

namespace NCL
{
	namespace CSC3222
	{
		class CollisionVolume
		{
			public:
			CollisionVolume(Maths::Vector2 position, Maths::Vector2 offset, int type);
			~CollisionVolume();

			int getType() { return type; }
			Maths::Vector2 getOffset() { return offset; }
			virtual Maths::Vector2 getPosition() = 0;
			void updatePos(Maths::Vector2 newPos) { position = newPos; }

			protected:
			int type = 0;
			Maths::Vector2 position;
			Maths::Vector2 offset;
		};
	}
}

