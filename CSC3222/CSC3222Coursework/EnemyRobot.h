#pragma once
#include "Robot.h"

namespace NCL
{
	namespace CSC3222
	{
		class EnemyRobot : public Robot
		{

			public:
			EnemyRobot();
			~EnemyRobot();

			bool UpdateObject(float dt) override;

			protected:
			float	thinkTime;
			bool	moving;
			float	testSpeed = 6;
			Vector2 direction;
		};
	}
}

