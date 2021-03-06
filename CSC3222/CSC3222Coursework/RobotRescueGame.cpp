#pragma once
#include "RobotRescueGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerRobot.h"
#include "EnemyRobot.h"
#include "CollectableRobot.h"
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"
#include "CollisionManager.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

int RobotRescueGame::robotCount = 0;
CircleCollisionVolume;
RectangleCollisionVolume;
bool yes = false;

RobotRescueGame::RobotRescueGame()
{
	renderer = new GameSimsRenderer();
	texManager = new TextureManager();
	physics = new GameSimsPhysics();

	SimObject::InitObjects(this, texManager);
	InitialiseGame();
	collisionManager = new CollisionManager(this, currentMap);
	physics->SetCollisionManager(collisionManager);
}

RobotRescueGame::~RobotRescueGame()
{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
	delete collisionManager;
}

void RobotRescueGame::Update(const float dt)
{
	spawnTimer += dt;
	
	// move to function!
	//BEGIN
	if (spawnTimer >= spawnDelay)
	{
		enemyRobotSpawn = true;
		spawnTimer = 0.0f;
	}

	if (enemyRobotSpawn && robotCount < maxEnemies)
	{
		float spawnX = MAX_X - 5.0f + 0;
		float spawnY = MAX_Y - 5.0f + 0;

		float spawnTopX = MAX_X/1.25f;
		float spawnTopY = MAX_Y/4.3f;
		
		(rand() % 2 == 0) ? AddEnemyRobot(Vector2(spawnX, spawnY)) 
			: AddEnemyRobot(Vector2(spawnTopX, spawnTopY));
		
		robotCount++;
		enemyRobotSpawn = !enemyRobotSpawn;
	}
	//END

	for (auto i : newObjects)
	{
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;

	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); )
	{
		SimObject* o = *i;
		if (o && !o->UpdateObject(dt))
		{ //object has said its finished with
			physics->RemoveCollider(o->GetCollider());
			physics->RemoveRigidBody(o);
			renderer->RemoveSimObject(o);
			delete *i;
			i = gameObjects.erase(i);
		}
		else
		{
			o->DrawObject(*renderer);
			++i;
		}
	}

	renderer->DrawString("Robot Rescue! ", Vector2(380, 700));

	renderer->DrawString("Current Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives: " + std::to_string(lives), Vector2(850, 10));

	renderer->Render();
}

void RobotRescueGame::InitialiseGame()
{
	delete currentMap;
	for (auto o : gameObjects)
	{
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("GameSimsRobotMap.txt", gameObjects, *texManager, physics);

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	testRobot = new PlayerRobot();
	AddNewObject(testRobot);

	for (int i = 0; i < 5; ++i)
	{
		float randomX = 32.0f + (rand() % MAX_X);
		float randomY = 32.0f + (rand() % MAX_Y);
		AddCollectableRobot(Vector2(randomX, randomY));
	}

	gameTime = 0;
	currentScore = 0;
	lives = 3;
}

void RobotRescueGame::AddNewObject(SimObject* object)
{
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider())
	{
		physics->AddCollider(object->GetCollider());
	}
}

void RobotRescueGame::AddEnemyRobot(const Vector2& position)
{
	EnemyRobot* robot = new EnemyRobot();

	robot->SetPosition(position);
	Vector2 offset = { 8,24 };
	robot->SetCollider(new CircleCollisionVolume(position, Vector2(8, 24), 8));
	robot->GetCollider()->setTag(ColliderTag::Enemy);
	robot->addPlayer(testRobot->GetCollider());

	AddNewObject(robot);
}

void RobotRescueGame::AddCollectableRobot(const Vector2& position)
{
	CollectableRobot* robot = new CollectableRobot();
	
	robot->SetMass(3);
	robot->SetPosition(position);
	Vector2 offset = { 8,8 };
	robot->SetCollider(new CircleCollisionVolume(position, offset, 8));
	robot->GetCollider()->setTag(ColliderTag::Collectible);
	

	AddNewObject(robot);
}