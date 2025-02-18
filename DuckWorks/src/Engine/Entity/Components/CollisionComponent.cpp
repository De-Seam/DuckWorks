#include "Precomp.h"
#include "Engine/Entity/Components/CollisionComponent.h"

// Engine includes
#include "Engine/Engine/Engine.h"
#include "Engine/World/World.h"

using namespace DC;

// CollisionComponent
RTTI_CLASS_DEFINITION(CollisionComponent, ClassAllocator)

Json CollisionComponent::Serialize()
{
	Json json = Base::Serialize();

	if (!mCollisionObjectHandle.IsValid())
		return json;

	CollisionObject& collision_object = gEngine.GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
	json["CollisionObject"] = collision_object.Serialize();

	return json;
}

void CollisionComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	GetEntity()->GetWorld()->GetCollisionWorld()->DeserializeCollisionObject(mCollisionObjectHandle, inJson["CollisionObject"]);
}

CollisionComponent::CollisionComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	CollisionObject::ConstructParameters construct_params;
	construct_params.mType = inConstructParameters.mType;
	construct_params.mBlocking = inConstructParameters.mBlocking;
	construct_params.mOnCollisionFunction = inConstructParameters.mOnCollisionFunction;
	construct_params.mEntity = GetEntity();
	construct_params.mTransform = GetWorldTransform();

	mCollisionObjectHandle = GetEntity()->GetWorld()->GetCollisionWorld()->CreateCollisionObject(construct_params);
	GetEntity()->RegisterMessageListener(this, &CollisionComponent::OnPreEntityPositionUpdated);
	GetEntity()->RegisterMessageListener(this, &CollisionComponent::OnPreEntityRotationUpdated);
}

CollisionComponent::~CollisionComponent()
{
	GetEntity()->GetWorld()->GetCollisionWorld()->DestroyCollisionObject(mCollisionObjectHandle);
	GetEntity()->UnregisterMessageListener(this, &CollisionComponent::OnPreEntityPositionUpdated);
	GetEntity()->UnregisterMessageListener(this, &CollisionComponent::OnPreEntityRotationUpdated);
}

void CollisionComponent::OnPreEntityPositionUpdated(MsgPreEntityPositionUpdated& ioMsg)
{
	PROFILE_SCOPE(CollisionComponent::OnPreEntityPositionUpdated)
	ioMsg.mNewPosition = GetEntity()->GetWorld()->GetCollisionWorld()->MoveTo(mCollisionObjectHandle, ioMsg.mNewPosition + GetLocalOffset().mPosition).mPosition - GetLocalOffset().mPosition;
}

void CollisionComponent::OnPreEntityRotationUpdated(MsgPreEntityRotationUpdated& ioMsg)
{
	PROFILE_SCOPE(CollisionComponent::OnPreEntityRotationUpdated)
	ioMsg.mNewRotation = GetEntity()->GetWorld()->GetCollisionWorld()->MoveTo(mCollisionObjectHandle, NullOpt, ioMsg.mNewRotation + GetLocalOffset().mRotation).mRotation - GetLocalOffset().mRotation;
}

CollisionObject& CollisionComponent::GetCollisionObject() const
{
	return GetEntity()->GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
}

void CollisionComponent::CalculateWorldTransform()
{
	Base::CalculateWorldTransform();

	GetEntity()->GetWorld()->GetCollisionWorld()->TeleportTransform(mCollisionObjectHandle, GetWorldTransform());
}
