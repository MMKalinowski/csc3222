#pragma once
namespace NCL
{
	namespace CSC3222
	{
		class CollisionVolume
		{
			public:
			CollisionVolume();
			~CollisionVolume();

			int getType() { return type; }

			protected:
			int type = 0;
		};
	}
}

