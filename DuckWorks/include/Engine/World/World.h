#pragma once
#include <DuckCore/RTTI/RTTIClass.h>
#include <DuckCore/Threads/Mutex.h>

#include "Core/Utilities/Utilities.h"

// Engine includes

#include <DuckCore/RTTI/Ref.h>

#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Entity.h"

class TextureRenderComponent;
class b2World;

enum class EWorldState : uint8
{
	Uninitialized,
	BeginningPlay,
	Playing,
	EndingPlay,
	EndedPlay
};

class World : public DC::RTTIClass
{
	RTTI_CLASS(World, RTTIClass)

public:
	virtual Json SerializeIgnoreEntities() const;

	using Base::Base;

	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	void BeginPlay();
	void EndPlay();

	template<typename taType>
	DC::Ref<taType> CreateEntity(const DC::String& inName = taType::sGetClassName());
	void AddEntity(const DC::Ref<Entity>& inEntity);
	void DestroyEntity(const DC::Ref<Entity>& inEntity);

	[[nodiscard]] Array<DC::Ref<Entity>>& GetEntities() { return mEntities; }
	[[nodiscard]] const Array<DC::Ref<Entity>>& GetEntities() const { return mEntities; }

	TextureRenderComponent* GetTextureRenderComponentAtLocationSlow(Vec2 inWorldLocation) const;

	CollisionWorld* GetCollisionWorld() const { return mCollisionWorld.get(); }
	EWorldState GetState() const { return mState; }
	bool HasBegunPlay() const { return mState == EWorldState::Playing || mState == EWorldState::BeginningPlay; }

private:
	void UpdateEntities(float inDeltaTime);
	void UpdateComponents(float inDeltaTime);
	void AddEntities();
	void DestroyEntities();

	EWorldState mState = EWorldState::Uninitialized;

	enum class EUpdateState : uint8
	{
		PreUpdate,
		Updating
	};

	EUpdateState mUpdateState = EUpdateState::PreUpdate;

	UniquePtr<CollisionWorld> mCollisionWorld = nullptr;

	Array<DC::Ref<Entity>> mEntities = {};

	Array<DC::Ref<Entity>> mEntitiesToAdd = {};
	DC::Mutex mEntitiesToAddMutex = {};

	Array<DC::Ref<Entity>> mEntitiesToRemove = {};
	DC::Mutex mEntitiesToRemoveMutex = {};

	int32 mVelocityIterations = 6;
	int32 mPositionIterations = 2;
	int32 mPhysicsUpdateFrequency = 60; //60 hz
	float mPhysicsTimeStep = 1.0f / SCast<float>(mPhysicsUpdateFrequency);
	float mPhysicsTimeAccumulator = 0.0f;

private:
	friend class BaseEntity;
};

template<typename taType>
DC::Ref<taType> World::CreateEntity(const DC::String& inName)
{
	static_assert(std::is_base_of_v<Entity, taType>);
	typename taType::ConstructParameters params;
	params.mWorld = this;
	params.mName = inName;
	DC::Ref<taType> entity = {};
	AddEntity(entity, inName);
	return entity;
}
