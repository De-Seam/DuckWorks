#pragma once
#include "Engine/Entity/Components/EntityComponent.h"

// Core includes
#include "Core/RTTI/Messages.h"

struct CollisionComponent : public EntityComponent
{
	RTTI_CLASS(CollisionComponent, EntityComponent, ClassAllocator)

	struct ConstructParameters : public Base::ConstructParameters
	{
		CollisionObject::ConstructParameters mCollisionObjectConstructParameters = {};
	};

	CollisionComponent(const ConstructParameters& inConstructParameters = {});
	virtual ~CollisionComponent();

	void OnPreEntityTransformUpdated(MsgPreEntityTransformUpdated& ioMsg);

private:
	CollisionObjectHandle mCollisionObjectHandle;
};