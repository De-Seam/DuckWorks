#pragma once
#include "Core/RTTI/RefObject.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Entity.h"

class b2World;

class World : public RTTIBaseClass
{
	RTTI_CLASS(World, RTTIBaseClass, StandardAllocator)

public:
	virtual Json SerializeIgnoreEntities() const;

	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	World(const ConstructParameters& inConstructParameters = {});

	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	void BeginPlay();
	void EndPlay();

	template<typename taType>
	Ref<taType> CreateEntity(const String& inName = taType::sGetClassName());
	void AddEntity(const Ref<Entity>& inEntity);
	void DestroyEntity(const Ref<Entity>& inEntity);

	[[nodiscard]] Array<Ref<Entity>>& GetEntities() { return mEntities; }
	[[nodiscard]] const Array<Ref<Entity>>& GetEntities() const { return mEntities; }

	Optional<Ref<Entity>> GetEntityAtLocationSlow(fm::vec2 inWorldLocation);

	CollisionWorld* GetCollisionWorld() const { return mCollisionWorld.get(); }
	bool HasBegunPlay() const { return mBegunPlay; }

private:
	void UpdateEntities(float inDeltaTime);
	void AddEntities();
	void DestroyEntities();

private:
	UniquePtr<CollisionWorld> mCollisionWorld = nullptr;

	Array<Ref<Entity>> mEntities = {};

	Array<Ref<Entity>> mEntitiesToAdd = {};
	UniqueMutex mEntitiesToAddMutex = {};

	Array<Ref<Entity>> mEntitiesToRemove = {};
	UniqueMutex mEntitiesToRemoveMutex = {};

	int32 mVelocityIterations = 6;
	int32 mPositionIterations = 2;
	int32 mPhysicsUpdateFrequency = 60; //60 hz
	float mPhysicsTimeStep = 1.0f / SCast<float>(mPhysicsUpdateFrequency);
	float mPhysicsTimeAccumulator = 0.0f;

	bool mBegunPlay = false;

private:
	friend class BaseEntity;
};

template<typename taType>
Ref<taType> World::CreateEntity(const String& inName)
{
	static_assert(std::is_base_of_v<Entity, taType>);
	typename taType::ConstructParameters params;
	params.mWorld = this;
	params.mName = inName;
	Ref<taType> entity = {};
	AddEntity(entity, inName);
	return entity;
}
