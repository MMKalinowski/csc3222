#pragma once
#include "RigidBody.h"
#include "../../Common/Vector2.h"

namespace NCL
{
	namespace CSC3222
	{
		enum ColliderTag
		{
			Free,
			Player,
			Terrain,
			Slowdown,
			Bullet,
			Enemy,
			Collectible
		};

		class CollisionVolume
		{
			public:
			CollisionVolume(Maths::Vector2 position, Maths::Vector2 offset, int type, RigidBody* = nullptr);
			~CollisionVolume();

			int getType() { return type; }
			void setOffset(Maths::Vector2 off) { offset = off; }
			Maths::Vector2 getOffset() { return offset; }
			virtual Maths::Vector2 getPosition() = 0; //change from pure to normal
			void updatePos(Maths::Vector2 newPos) { position = newPos; }

			void setTag(ColliderTag newTag) { tag = newTag; }
			ColliderTag getTag() { return tag; }
			void setRigidBody(RigidBody* const rb) { this->rb = rb; }
			RigidBody* getRigidBody() const { return rb; }

			protected:
			int type = 0;
			Maths::Vector2 position;
			Maths::Vector2 offset;
			ColliderTag tag = ColliderTag::Free;
			RigidBody* rb;
		};
	}
}

