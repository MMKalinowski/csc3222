#pragma once
#include "CollisionVolume.h"
#include "RobotRescueGame.h"
#include "GameMap.h"

#include "../../Common/Vector2.h"

namespace NCL
{
	namespace CSC3222
	{
		class RobotRescueGame;
		struct Collision;

		class CollisionManager
		{
			public:
			CollisionManager(RobotRescueGame* game, GameMap* map)
			: game(game)
			, map(map)
			{}

			~CollisionManager() {}

			bool shouldCollide(CollisionVolume*, CollisionVolume*, Collision);

			private:
			RobotRescueGame* game;
			GameMap* map;
		};
	}
}