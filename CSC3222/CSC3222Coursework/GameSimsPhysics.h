#pragma once
#include <vector>
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"
#include "../../Common/Vector2.h"

namespace NCL
{
	namespace CSC3222
	{
		class RigidBody;
		class CollisionVolume;

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
			//void Integration(const float dt);
			void IntegrateAccel(const float dt);
			void IntegrateVel(const float dt);
			
			void CollisionDetection(const float dt);
			bool CheckCollision(CollisionVolume* l, CollisionVolume* r);
			bool CircleCircle(CircleCollisionVolume* l, CircleCollisionVolume* r);
			bool RectRect(RectangleCollisionVolume* l, RectangleCollisionVolume* r);
			bool CircleRect(CollisionVolume* l, CollisionVolume* r);

			float timeUntilUpdate = 0;
			const float subDT = 1.0f / 120.f;

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
		};
	}
}

