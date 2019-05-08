#pragma once
#include "../../Common/Vector2.h"

namespace NCL
{
	using namespace Maths;
	namespace CSC3222
	{
		class CollisionVolume;

		class RigidBody
		{
			friend class GameSimsPhysics;
			public:
			RigidBody(CollisionVolume* = nullptr);
			~RigidBody();

			virtual void SetCollider(CollisionVolume* collider)
			{
				this->collider = collider;
			}

			virtual CollisionVolume* GetCollider() const
			{
				return this->collider;
			}

			Vector2 GetPosition() const
			{
				return position;
			}

			void SetPosition(const Vector2& newPosition)
			{
				position = newPosition;
			}

			Vector2 GetVelocity() const
			{
				return velocity;
			}

			void SetVelocity(const Vector2& newVelocity)
			{
				velocity = newVelocity;
			}

			void AddForce(const Vector2& newForce)
			{
				force += newForce;
			}

			float GetMass()
			{
				return inverseMass;
			}

			void SetMass(float invMass)
			{
				this->inverseMass = invMass;
			}

			float GetElasticity()
			{
				return elasticity;
			}

			protected:
			Vector2 position;
			Vector2 velocity;
			Vector2 force;

			float inverseMass;
			float elasticity;

			CollisionVolume* collider;
		};
	}
}

#include "CollisionVolume.h"
