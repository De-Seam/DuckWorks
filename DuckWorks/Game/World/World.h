#pragma once
// Core includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>

// Engine includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/StaticArray.h>

#include <Engine/Objects/Object.h>

class Entity;
class World;

enum class EWorldTickFrequency : uint8
{
	Hz1,
	Hz15,
	Hz30,
	Hz60,

	Num
};

class WorldTickHandle : public DC::Handle
{
public:
	virtual ~WorldTickHandle() override;

private:
	WorldTickHandle(int inID, World& inWorld, EWorldTickFrequency inTickFrequency) :
		Handle(inID),
		mWorld(&inWorld),
		mTickFrequency(inTickFrequency)
	{}

	World* mWorld = nullptr;
	EWorldTickFrequency mTickFrequency = EWorldTickFrequency::Hz1;

	friend class World;
};

class World : public Object
{
	RTTI_CLASS(World, Object)
public:
	World();
	virtual ~World() override;

	void Update(float inDeltaTime);

	void AddEntity(Entity& inEntity);
	void RemoveEntity(Entity& inEntity);

	[[nodiscard]]
	DC::Ref<WorldTickHandle> RegisterTickCallback(EWorldTickFrequency inTickFrequency, std::function<void(float)> inCallback);

private:
	void UnregisterTickCallback(const WorldTickHandle& inHandle);

	DC::Array<DC::Ref<Entity>> mEntities;

	struct TickCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	DC::StaticArray<DC::Array<TickCallback>, static_cast<int>(EWorldTickFrequency::Num)> mTickFrequencyToCallbacks;
	int mLastTickHandleID = 0;

	DC::StaticArray<float, static_cast<uint>(EWorldTickFrequency::Num)> mHzToTimeSinceUpdate = { 0.0f };
	DC::StaticArray<float, static_cast<uint>(EWorldTickFrequency::Num)> mHzToTargetSeconds = { 1.0f, 1.0f / 15.0f, 1.0f / 30.0f, 1.0f / 60.0f };

	friend class WorldTickHandle;
};
