#include <Game/World/World.h>

// Game includes
#include <Game/Entity/Entity.h>

WorldTickHandle::~WorldTickHandle()
{
	gAssert(mWorld != nullptr);
	mWorld->UnregisterTickCallback(*this);
}

World::World() = default;

World::~World() = default;

void World::Update(float inDeltaTime)
{
	for (float& time_since_update : mHzToTimeSinceUpdate)
		time_since_update += inDeltaTime;

	for (int i = 0; i < mTickFrequencyToCallbacks.Length(); i++)
	{
		float& time_since_update = mHzToTimeSinceUpdate[static_cast<uint8>(mHzToTimeSinceUpdate[i])];
		float target_seconds = mHzToTargetSeconds[i];

		while (time_since_update >= target_seconds)
		{
			DC::Array<TickCallback>& tick_callbacks = mTickFrequencyToCallbacks[i];

			for (const TickCallback& callback : tick_callbacks)
				callback.mCallback(target_seconds);

			time_since_update -= target_seconds;
		}
	}
}

void World::AddEntity(Entity& inEntity)
{
	mEntities.Add(&inEntity);
}

void World::RemoveEntity(Entity& inEntity)
{
	const int index = mEntities.Find(&inEntity);
	gAssert(index != -1);
	mEntities.SwapRemove(index);
}

DC::Ref<WorldTickHandle> World::RegisterTickCallback(EWorldTickFrequency inTickFrequency, std::function<void(float)> inCallback)
{
	int next_id = mLastTickHandleID++;
	gAssert(next_id >= 0, "IDs ran out");

	DC::Ref<WorldTickHandle> handle = new WorldTickHandle(next_id, *this, inTickFrequency);
	mTickFrequencyToCallbacks[gStaticCast<int>(inTickFrequency)].Emplace(next_id, inCallback);

	return handle;
}

void World::UnregisterTickCallback(const WorldTickHandle& inHandle)
{
	DC::Array<TickCallback>& callbacks = mTickFrequencyToCallbacks[gStaticCast<int>(inHandle.mTickFrequency)];
	const int index = callbacks.FindIf([&](const TickCallback& inItem) { return inItem.mID == inHandle.GetID(); });
	gAssert(index != -1);
	callbacks.SwapRemove(index);
}
