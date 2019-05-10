#pragma once
#include "Robot.h"

namespace NCL
{
	namespace CSC3222
	{
		enum State
		{
			Idle,
			Attack,
			Defend
		};

		class EnemyRobot : public Robot
		{

			public:
			EnemyRobot();
			~EnemyRobot();

			bool UpdateObject(float dt) override;
			void addPlayer(CollisionVolume*);

			protected:
			float	thinkTime;
			bool	moving;
			float	testSpeed = 6;
			Vector2 direction;
			State	currentState;
			CollisionVolume* player;

			private:
			void FSM(float dt, CollisionVolume* player);
		};
	}
}

