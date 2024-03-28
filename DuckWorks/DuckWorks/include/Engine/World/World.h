#pragma once
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/Entity.h"
#include "Engine/Collision/CollisionWorld.h"

// External includes
#include <External/entt/entt.hpp>

class b2World;

class World : public RTTIBaseClass
{
	RTTI_CLASS(World, RTTIBaseClass)

public:
	virtual Json SerializeIgnoreEntities() const;

	World();
	virtual ~World();

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	void BeginPlay();

	void UpdateEntities(float inDeltaTime);
	void DestroyEntities();

	template<typename taType>
	SharedPtr<taType> CreateEntity(const String& inName);
	EntityPtr AddEntity(const EntityPtr& inEntity, const String& inName);

	[[nodiscard]] Array<EntityPtr>& GetEntities() { return mEntities; }
	[[nodiscard]] const Array<EntityPtr>& GetEntities() const { return mEntities; }
	[[nodiscard]] entt::registry& GetRegistry() { return mRegistry; }

	EntityPtr GetEntityAtLocationSlow(fm::vec2 inWorldLocation);

	CollisionWorld* GetCollisionWorld() const { return mCollisionWorld.get(); }

private:
	entt::registry mRegistry = {};
	Mutex mRegistryMutex = {};

	UniquePtr<CollisionWorld> mCollisionWorld = nullptr;

	Array<EntityPtr> mEntities = {};
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
std::shared_ptr<taType> World::CreateEntity(const String& inName)
{
	static_assert(std::is_base_of_v<Entity, taType>);

	std::shared_ptr<taType> entity = std::make_shared<taType>(this);
	entity->mThisWeakPtr = entity;
	AddEntity(std::static_pointer_cast<Entity>(entity), inName);
	return entity;
}
