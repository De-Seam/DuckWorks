#pragma once
#include "Core/CoreBase.h"

class Entity;

class EntityComponent : public RTTIBaseClass
{
	RTTI_CLASS(EntityComponent, RTTIBaseClass)

public:
	EntityComponent() = default;
	virtual ~EntityComponent() override {}

	virtual void Init() {}
	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update([[maybe_unused]] float inDeltaTime) {}

	Entity* GetEntity() const { return mEntity; }

private:
	Entity* mEntity = nullptr;

	friend class EntityComponentManager;
};
