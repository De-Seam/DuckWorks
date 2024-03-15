#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Collision/Grid.h"

class Entity;

struct Chunk
{
	Grid mGrid;

	fm::vec2 mTopRight; ///< The anchor points
	Array<SharedPtr<Entity>> mEntities; ///< The entities. All their positions are relative to mTopRight
};

class ChunkManager
{
	Array<Array<Chunk>> mChunks;
};
