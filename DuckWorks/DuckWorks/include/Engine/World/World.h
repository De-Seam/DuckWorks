#pragma once
#include "Core/Utilities/RefObject.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Entity.h"

// External includes
#include <External/entt/entt.hpp>

class b2World;

class World : public RTTIBaseClass
{
	RTTI_CLASS(World, RTTIBaseClass)

public:
	virtual Json SerializeIgnoreEntities() const;

	World();
	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	void BeginPlay();
	void EndPlay();

	void UpdateEntities(float inDeltaTime);
	void DestroyEntities();

	template<typename taType>
	Ref<taType> CreateEntity(const String& inName);
	Ref<Entity> AddEntity(const Ref<Entity>& inEntity, const String& inName, Entity::InitParams inInitParams = {});

	[[nodiscard]] Array<Ref<Entity>>& GetEntities() { return mEntities; }
	[[nodiscard]] const Array<Ref<Entity>>& GetEntities() const { return mEntities; }
	[[nodiscard]] entt::registry& GetRegistry() { return mRegistry; }

	Optional<Ref<Entity>> GetEntityAtLocationSlow(fm::vec2 inWorldLocation);

	CollisionWorld* GetCollisionWorld() const { return mCollisionWorld.get(); }

private:
	entt::registry mRegistry = {};
	Mutex mRegistryMutex = {};

	UniquePtr<CollisionWorld> mCollisionWorld = nullptr;

	Array<Ref<Entity>> mEntities = {};
	Mutex mEntitiesMutex = {};

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

	Ref<taType> entity = {this};
	AddEntity(entity, inName);
	return entity;
}
