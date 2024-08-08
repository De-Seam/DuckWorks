#pragma once
#include "Engine/Entity/Components/EntityComponent.h"

// Core includes
#include "Core/RTTI/Messages.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Components.h"

struct CollisionComponent : public WorldComponent
{
	RTTI_CLASS(CollisionComponent, WorldComponent)
	struct ConstructParameters : public Base::ConstructParameters
	{
		CollisionObject::EType mType = CollisionObject::EType::Static;
		bool mBlocking = true;

		OnCollisionFunc mOnCollisionFunction = nullptr;
	};

	CollisionComponent(const ConstructParameters& inConstructParameters = {});
	virtual ~CollisionComponent() override;

	void OnPreEntityPositionUpdated(MsgPreEntityPositionUpdated& ioMsg);
	void OnPreEntityRotationUpdated(MsgPreEntityRotationUpdated& ioMsg);

	CollisionObject& GetCollisionObject() const;

protected:
	virtual void CalculateWorldTransform() override;

private:
	CollisionObjectHandle mCollisionObjectHandle;
};
