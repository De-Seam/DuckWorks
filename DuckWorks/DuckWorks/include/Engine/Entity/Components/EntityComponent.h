#pragma once
#include "Core/CoreBase.h"

class EntityComponent : public RTTIBaseClass
{
	RTTI_CLASS(EntityComponent, RTTIBaseClass)

public:
	EntityComponent() {}
	virtual ~EntityComponent() {}

	virtual void Init() {};
	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update([[maybe_unused]] float inDeltaTime) {}
};
