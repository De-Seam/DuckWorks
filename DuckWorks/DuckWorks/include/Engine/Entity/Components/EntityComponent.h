#pragma once
#include "Core/CoreBase.h"

class Entity;

class EntityComponent : public RTTIBaseClass
{
	RTTI_CLASS(EntityComponent, RTTIBaseClass)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		Entity* mEntity = nullptr;
	};

	EntityComponent(const ConstructParameters& inConstructParameters)
		: Base(inConstructParameters), mEntity(inConstructParameters.mEntity)
	{
		gAssert(mEntity != nullptr, "Entity was not set!");
	}

	virtual ~EntityComponent() override {}

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
