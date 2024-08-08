#pragma once
#include "Engine/Entity/Components/EntityComponent.h"

// Core includes
#include "Core/RTTI/Messages.h"

// Engine includes

class LuaResource;

class ScriptComponent : public EntityComponent
{
	RTTI_CLASS(ScriptComponent, EntityComponent)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		SharedPtr<LuaResource> mUpdateScript = nullptr;
	};

	ScriptComponent(const ConstructParameters& inConstructParameters = {});

	virtual void Update(float inDeltaTime) override;

private:
	SharedPtr<LuaResource> mUpdateScript = {};
};
