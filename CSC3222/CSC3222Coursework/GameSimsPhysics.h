#pragma once
#include <vector>
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"
#include "../../Common/Vector2.h"
#include "../../Common/Maths.h"

namespace NCL
{
	namespace CSC3222
	{
		class RigidBody;
		class CollisionVolume;

		struct Collision
		{
			Vector2 normal;
			float penetration;
			bool occured() const { return penetration > 0; }
		};

		class GameSimsPhysics
		{
			public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(const float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

			protected:
			void IntegrateAccel(const float dt);
			void IntegrateVel(const float dt);
			
			void CollisionDetection(const float dt);
			Collision CheckCollision(CollisionVolume* l, CollisionVolume* r);
			Collision CircleCircle(CircleCollisionVolume* l, CircleCollisionVolume* r);
			Collision RectRect(RectangleCollisionVolume* l, RectangleCollisionVolume* r);
			Collision CircleRect(CollisionVolume* l, CollisionVolume* r);

			void ResolveCollision(CollisionVolume* l, CollisionVolume* r, Collision col);

			float timeUntilUpdate = 0;
			const float subDT = 1.0f / 120.f;
			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;

			//float clamp(const float val, const float minv, const float maxv) { return max(minv, min(val, maxv)); }
			/*
			float min(const float l, const float r)
			{
				return l > r ? r : l;
			}
			
			float max(const float l, const float r) 
			{
				return std::abs(l - r) > Maths::EPSILON ? l : r;
			}*/
		};
	}
}

