#pragma once
#include "Core/CoreBase.h"

// Each chunk is 10k x 10k
#define CHUNK_WIDTH 10000
#define CHUNK_HEIGHT 10000

class Entity;

struct Chunk
{
	fm::vec2 mTopRight; ///< The anchor points
	Array<SharedPtr<Entity>> mEntities; ///< The entities. All their positions are relative to mTopRight
};

class ChunkManager
{
	Array<Array<Chunk>> mChunks;
};