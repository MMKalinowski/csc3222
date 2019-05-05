#pragma once
#include "RectangleCollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

RectangleCollisionVolume::RectangleCollisionVolume(int offset, int x, int y) : CollisionVolume()
{
	this->x_size = x;
	this->y_size = y;
	this->offset = offset;
}
RectangleCollisionVolume::~RectangleCollisionVolume()
{

}