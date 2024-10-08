#pragma once
#include "Core/RTTI/RefObject.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Entity.h"

class MsgEntityUpdateFrequencyChanged;
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

class World : public RTTIBaseClass
{
	RTTI_CLASS(World, RTTIBaseClass, StandardAllocator)

public:
	virtual Json SerializeIgnoreEntities() const;
	virtual void DeserializeIgnoreEntities(const Json& inJson);

	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	World(const ConstructParameters& inConstructParameters = {});

	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render();

	void BeginPlay();
	void EndPlay();

	template<typename taType>
	Ref<taType> CreateEntity(const String& inName = taType::sGetClassName());
	void AddEntity(const Ref<Entity>& inEntity);
	void DestroyEntity(const Ref<Entity>& inEntity);

	[[nodiscard]] Array<Ref<Entity>>& GetEntities() { return mEntities; }
	[[nodiscard]] const Array<Ref<Entity>>& GetEntities() const { return mEntities; }

	TextureRenderComponent* GetTextureRenderComponentAtLocationSlow(Vec2 inWorldLocation) const;

	CollisionWorld* GetCollisionWorld() const { return mCollisionWorld.get(); }
	EWorldState GetState() const { return mState; }
	bool HasBegunPlay() const { return mState == EWorldState::Playing || mState == EWorldState::BeginningPlay; }

	void OnEntityUpdateFrequencyChanged(const MsgEntityUpdateFrequencyChanged& inMsg);

private:
	void UnregisterEntityForUpdates(Entity& inEntity, Entity::EUpdateFrequency inOldUpdateFrequency);
	void RegisterEntityForUpdates(Entity& inEntity, Entity::EUpdateFrequency inUpdateFrequency);

	void UpdateObjects(float inDeltaTime);

	void UpdateFrameEntities(float inDeltaTime);
	void UpdateTickEntities();
	void UpdateSecondEntities();

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

	Array<Ref<Entity>> mEntities = {};
	StaticArray<Array<Entity*>, SCast<uint8>(Entity::EUpdateFrequency::None)> mEntitiesToUpdate;

	Array<Ref<Entity>> mEntitiesToAdd = {};
	UniqueMutex mEntitiesToAddMutex = {};

	Array<Ref<Entity>> mEntitiesToRemove = {};
	UniqueMutex mEntitiesToRemoveMutex = {};

	float mTickRate = 1.0f / 60.0f; ///< 60 hz
	float mTimeSinceTick = 0.0f;
	float mTimeSinceFullSecond = 0.0f;

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
