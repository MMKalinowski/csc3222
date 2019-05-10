#pragma once
#include <vector>
#include "CollisionManager.h"


namespace NCL
{
	namespace Maths
	{
		class Vector2;
	}
	namespace CSC3222
	{
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class PlayerRobot;
		class TextureManager;
		class CollisionManager;

		const static unsigned int MAX_X = 416;
		const static unsigned int MAX_Y = 256;
		const static float spawnDelay = 1.0f;
		const static int maxEnemies = 10;

		class RobotRescueGame
		{
			public:
			RobotRescueGame();
			~RobotRescueGame();

			void Update(const float dt);

			void AddNewObject(SimObject* object);

			protected:
			void InitialiseGame();

			void AddEnemyRobot(const Maths::Vector2& position);
			void AddCollectableRobot(const Maths::Vector2& position);

			TextureManager*		texManager;
			GameSimsRenderer*	renderer;
			GameSimsPhysics*	physics;
			GameMap* currentMap;
			CollisionManager* collisionManager;

			PlayerRobot* testRobot;

			float gameTime;
			float spawnTimer = 0.0f;
			static int robotCount;

			int currentScore;
			int lives;
			bool enemyRobotSpawn = true;

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;
		};
	}
}

