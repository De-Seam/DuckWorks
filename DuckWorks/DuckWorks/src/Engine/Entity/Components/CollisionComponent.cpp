#include "Precomp.h"
#include "Engine/Entity/Components/CollisionComponent.h"

// Engine includes
#include "Engine/Engine/Engine.h"
#include "Engine/World/World.h"

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
	construct_params.mTransform = GetEntity()->GetTransform();

	mCollisionObjectHandle = GetEntity()->GetWorld()->GetCollisionWorld()->CreateCollisionObject(construct_params);
	GetEntity()->RegisterMessageListener(this, &CollisionComponent::OnPreEntityTransformUpdated);
}

CollisionComponent::~CollisionComponent()
{
	GetEntity()->GetWorld()->GetCollisionWorld()->DestroyCollisionObject(mCollisionObjectHandle);
	GetEntity()->UnregisterMessageListener(this, &CollisionComponent::OnPreEntityTransformUpdated);
}

void CollisionComponent::OnPreEntityTransformUpdated(MsgPreEntityTransformUpdated& ioMsg)
{
	PROFILE_SCOPE(CollisionComponent::OnPreEntityTransformUpdated)
	gEngine.GetWorld()->GetCollisionWorld()->MoveTo(mCollisionObjectHandle, ioMsg.mNewTransform);
}

CollisionObject& CollisionComponent::GetCollisionObject() const
{
	return GetEntity()->GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
}
