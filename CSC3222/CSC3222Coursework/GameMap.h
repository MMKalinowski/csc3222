#pragma once
#include <string>
#include <vector>

#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

#include "GameSimsPhysics.h"

namespace NCL
{
	namespace Rendering
	{
		class TextureBase;
	}
	using namespace Maths;
	namespace CSC3222
	{
		class GameSimsRenderer;
		class SimObject;
		class TextureManager;

		enum MapTileType
		{
			Flat,
			Wall,
			Rough,
			MAX_TILES,
		};

		enum MapStructureType
		{
			RedTeleporter,
			GreenTeleporter,
			BlueTeleporter,
			//RobotHome,
		};

		struct StructureData
		{
			Vector2				startPos;
			MapStructureType	structureType;
		};

		struct Rect
		{
			int x;
			int y;
			int w;
			int h;
		};

		class GameMap
		{
			public:
			GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager, GameSimsPhysics* physics);
			~GameMap();

			void DrawMap(GameSimsRenderer& r);

			int GetMapWidth() const
			{
				return mapWidth;
			}

			int GetMapHeight() const
			{
				return mapHeight;
			}

			Vector2 teleportTo(MapStructureType type)
			{
				int i = 0;
				while (i < structureCount)
				{
					if (structureData[i].structureType == type)
					{
						Vector2 offset = {0,0};
						switch (type)
						{
							case MapStructureType::BlueTeleporter:
								offset = Vector2(-2 * 16, 16);
								break;
							case MapStructureType::RedTeleporter:
								offset = Vector2(2 * 16, 5 * 16);
								break;
							case MapStructureType::GreenTeleporter:
								offset = Vector2(3 * 16, -1 * 16);
								break;
						}
						return structureData[i].startPos + offset;
					}
					++i;
				}
				return Vector2(-20, -20);
			}

			private:
			Rect makeRectAt(int sx, int sy, MapTileType, std::vector<int>& used) const;
			
			protected:
			int mapWidth;
			int mapHeight;
			int structureCount;

			static Vector4 tileSpecs[MAX_TILES];

			Rendering::TextureBase*	tileTexture;
			MapTileType*	mapData;
			StructureData*	structureData;
			GameSimsPhysics* physics;

			void GenerateColliders(GameSimsPhysics*, MapTileType);
		};
	}
}

