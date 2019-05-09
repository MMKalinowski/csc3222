#pragma once
#include "CollisionVolume.h"
#include "GameSimsPhysics.h"
#include "RobotRescueGame.h"

#include "../../Common/Vector2.h"

namespace NCL
{
	namespace CSC3222
	{
		class CollisionManager
		{
			public:
			CollisionManager(GameSimsPhysics* phys)
			:physics(phys)
			{}

			~CollisionManager() {}

			bool shouldCollide(CollisionVolume*, CollisionVolume*, Collision);

			private:
			GameSimsPhysics* physics;
			RobotRescueGame* game;
		};
	}
}