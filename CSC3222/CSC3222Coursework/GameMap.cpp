#include "GameMap.h"
#include "GameSimsRenderer.h"
#include "TextureManager.h"
#include "CollisionVolume.h"
#include "CircleCollisionVolume.h"
#include "RectangleCollisionVolume.h"
#include "../../Common/Assets.h"
#include "../../Common/TextureLoader.h"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

//These are all in format: x pos, y pos, xsize, ysize
Vector4 GameMap::tileSpecs[MAX_TILES] = {
	Vector4(101,13,16,16),	//Flat
	Vector4(96,56,16,16),	//Wall
	Vector4(144,56,16,16),	//Rough
};

//Random rough looking tiles for variety
static Vector4 randomRoughTiles[4] = {
	Vector4(144,40,16,16),	//
	Vector4(144,56,16,16),	//
	Vector4(160,40,16,16),	//
	Vector4(160,56,16,16),	//
};

static Vector4 buildingTypes[4] = {
	Vector4(376,56,32,48),	//Red teleporter
	Vector4(376,112,32,48), //blue teleporter
	Vector4(376,168,32,48), //green teleporter
	Vector4(144,256,64,64)  //Robot Home
};


static std::vector<RectangleCollisionVolume*> wallColliders;

GameMap::GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager, GameSimsPhysics* physics)
{
	tileTexture = texManager.GetTexture("Colony.png");

	std::ifstream mapFile(Assets::DATADIR + filename);

	if (!mapFile)
	{
		std::cout << "GameMap can't be loaded in!" << std::endl;
		return;
	}

	mapFile >> mapWidth;
	mapFile >> mapHeight;

	mapData = new MapTileType[mapWidth * mapHeight];

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int tileIndex = (y * mapWidth) + x;

			char type = 0;

			mapFile >> type;

			mapData[tileIndex] = (MapTileType)(type - 48);
		}
	}
	mapFile >> structureCount;

	structureData = new StructureData[structureCount];

	for (int i = 0; i < structureCount; ++i)
	{
		int type = 0;
		int xTile = 0;
		int yTile = 0;

		mapFile >> type;
		mapFile >> xTile;
		mapFile >> yTile;

		structureData[i].structureType = (MapStructureType) type;
		structureData[i].startPos.x = float(xTile * 16); //explicit type conversion
		structureData[i].startPos.y = float(yTile * 16); //explicit type conversion

		ColliderTag tag;
		switch (type)
		{
			case MapStructureType::BlueTeleporter:
				tag = ColliderTag::Blue;
			case MapStructureType::RedTeleporter:
				tag = ColliderTag::Red;
			case MapStructureType::GreenTeleporter:
				tag = ColliderTag::Green;
			default:
				tag = ColliderTag::Home;
		}

		Vector4 building = buildingTypes[type];
		auto collider = new RectangleCollisionVolume(
			structureData[i].startPos, { building.z / 2, building.w / 2 },
			building.z, building.w, nullptr, tag
		);

		physics->AddCollider(collider);
		wallColliders.push_back(collider);
	}

	GenerateColliders(physics, MapTileType::Wall);
	GenerateColliders(physics, MapTileType::Rough);
}

GameMap::~GameMap()
{
	delete[] mapData;
	delete[] structureData;
}

void GameMap::DrawMap(GameSimsRenderer & r)
{
	int tileSize = 16;

	srand(0); //so we get consistent floor tiles

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int tileType = mapData[(y * mapWidth) + x];

			Vector4 tileData;

			if (tileType == MapTileType::Rough)
			{
				int randomFlatTile = rand() % 4;
				tileData = randomRoughTiles[randomFlatTile];
			}
			else
			{
				tileData = tileSpecs[tileType];
			}

			Vector2 texPos = Vector2(tileData.x, tileData.y);
			Vector2 texSize = Vector2(tileData.z, tileData.w);

			Vector2 screenPos = Vector2(float(x * 16), float(y * 16)); //explicit type conversion

			r.DrawTextureArea((OGLTexture*)tileTexture, texPos, texSize, screenPos, false);
		}
	}

	for (RectangleCollisionVolume* rcv : wallColliders)
	{
		r.DrawBox(rcv->getPosition(), rcv->getOffset());
	}

	for (int i = 0; i < structureCount; ++i)
	{
		Vector4 buildingData = buildingTypes[structureData[i].structureType];

		Vector2 texPos = Vector2(buildingData.x, buildingData.y);
		Vector2 texSize = Vector2(buildingData.z, buildingData.w);

		Vector2 screenPos = structureData[i].startPos;

		r.DrawTextureArea((OGLTexture*) tileTexture, texPos, texSize, screenPos, false);
	}
}

Rect GameMap::makeRectAt(int sx, int sy, MapTileType type, std::vector<int>& used) const
{
	Rect rect = {sx, sy, mapWidth - sx, 1};
	for (int y = sy; y < mapHeight; ++y)
	{
		if (used[sx + y * mapWidth] || mapData[sx + y * mapWidth] != type)
		{
			rect.h = y - sy;
			break;
		}
		for (int x = sx; x < sx + rect.w; ++x)
		{
			const int index = x + y * mapWidth;
			const bool isUsed = used[index];
			const bool isType = mapData[index] == type;
			if (isUsed || !isType)
			{
				rect.w = min(rect.w, x - sx);
				rect.h = y - sy;
				break;
			}
		}
		if (y == mapHeight - 1)
		{
			rect.h = y - sy + 1;
		}
	}
	return rect;
}

void makeAreaUsed(std::vector<int>& used, const Rect& area, int mapWidth)
{
	for (int y = area.y; y < area.y + area.h; ++y)
	{
		for (int x = area.x; x < area.x + area.w; ++x)
		{
			const int index = x + y * mapWidth;
			used[index] = 1;
		}
	}
}

void GameMap::GenerateColliders(GameSimsPhysics* physicsLoc, MapTileType type)
{
	const int size = mapWidth * mapHeight;
	std::vector<int> used(size);
	std::fill(used.begin(), used.end(), 0);

	ColliderTag tag;

	switch (type)
	{
		case MapTileType::Wall:
			tag = ColliderTag::Terrain;
			break;
		
		case MapTileType::Rough:
			tag = ColliderTag::Slowdown;
			break;
	}

	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			const int index = x + y * mapWidth;
			if (!used[index] && mapData[index] == type)
			{
				Rect r = makeRectAt(x, y, type, used);
				if (r.h && r.w)
				{
					makeAreaUsed(used, r, mapWidth);
					auto collider = new RectangleCollisionVolume(
						Vector2(r.x * 16, r.y * 16), Vector2(r.w * 8, r.h * 8),
						r.w * 16, r.h * 16, nullptr, tag
					);
					physicsLoc->AddCollider(collider);
					wallColliders.push_back(collider);
				}
			}
		}
	}
	std::cout << "Wall colliders: " << wallColliders.size() << std::endl;
}