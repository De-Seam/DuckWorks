#pragma once
#include "Core/CoreBase.h"

class Entity;

struct Chunk
{
	Vec2 mTopRight; ///< The anchor points
	Array<SharedPtr<Entity>> mEntities; ///< The entities. All their positions are relative to mTopRight
};

class ChunkManager
{
	Array<Array<Chunk>> mChunks;
};
