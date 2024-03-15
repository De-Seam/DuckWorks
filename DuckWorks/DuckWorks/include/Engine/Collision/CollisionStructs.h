#pragma once
#include "Core/CoreBase.h"

// Each chunk is 10k x 10k
#define CHUNK_WIDTH 10000
#define CHUNK_HEIGHT 10000

#define TILES_PER_CHUNK_X 20
#define TILES_PER_CHUNK_Y 20
#define TILE_WIDTH (CHUNK_WIDTH / TILES_PER_CHUNK_X)
#define TILE_HEIGHT (CHUNK_HEIGHT / TILES_PER_CHUNK_Y)

using CollisionObjectHandle = Handle<struct CollisionObjectHandleEmptyStruct>;

struct AABB
{
	fm::vec2 mMin, mMax;
};
