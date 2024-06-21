#include "Precomp.h"
#include "Engine/Entity/Components/CollisionComponent.h"

// Engine includes
#include "Engine/World/World.h"

// Game includes
#include "Game/App/App.h"

// CollisionComponent
RTTI_CLASS_DEFINITION(CollisionComponent, ClassAllocator)

Json CollisionComponent::Serialize()
{
	Json json = Base::Serialize();

	if (!mCollisionObjectHandle.IsValid())
		return json;

	CollisionObjectWrapper collision_object = gApp.GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
	json["CollisionObject"] = collision_object->Serialize();

	return json;
}

void CollisionComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	GetEntity()->GetWorld()->GetCollisionWorld()->DeserializeCollisionObject(mCollisionObjectHandle, inJson["CollisionObject"]);
}

CollisionComponent::CollisionComponent(const ConstructParameters& inConstructParameters = {})
	: Base(inConstructParameters),
{
	CollisionObject::ConstructParameters construct_params = inConstructParameters.mCollisionObjectConstructParameters;
	construct_params.mTransform = GetEntity()->GetTransform();
	GetEntity()->GetWorld()->GetCollisionWorld()->CreateCollisionObject(construct_params);
	GetEntity()->RegisterMessageListener(this, CollisionComponent::OnPreEntityTransformUpdated);
}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::OnPreEntityTransformUpdated(MsgPreEntityTransformUpdated& ioMsg)
{
	gApp.GetWorld()->GetCollisionWorld()->MoveTo(mCollisionObjectHandle, ioMsg.mNewTransform);
}
