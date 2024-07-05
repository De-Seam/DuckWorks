#pragma once
#include "Engine/Entity/Components/EntityComponent.h"

// Core includes
#include "Core/RTTI/Messages.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"

struct CollisionComponent : public EntityComponent
{
	RTTI_CLASS(CollisionComponent, EntityComponent, ClassAllocator)
	struct ConstructParameters : public Base::ConstructParameters
	{
		CollisionObject::EType mType = CollisionObject::EType::Static;
		bool mBlocking = true;
		Vec2 mHalfSize = {};

		OnCollisionFunc mOnCollisionFunction = nullptr;
	};

	CollisionComponent(const ConstructParameters& inConstructParameters = {});
	virtual ~CollisionComponent() override;

	void OnPreEntityPositionUpdated(MsgPreEntityPositionUpdated& ioMsg);
	void OnPreEntityRotationUpdated(MsgPreEntityRotationUpdated& ioMsg);

	CollisionObject& GetCollisionObject() const;

private:
	CollisionObjectHandle mCollisionObjectHandle;
};
