#pragma once
#include "Core/CoreBase.h"

class Entity;

class EntityComponent : public RTTIRefObject
{
	RTTI_CLASS(EntityComponent, RTTIRefObject)

public:
	// TODO: Set Entity
	virtual ~EntityComponent() override = default;

	virtual void Init() {}
	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update([[maybe_unused]] float inDeltaTime) {}

	Entity* GetEntity() const
	{
		gAssert(mEntity != nullptr, "Entity was not set!");
		return mEntity;
	}

private:
	Entity* mEntity = nullptr;

	friend class Entity;
};
