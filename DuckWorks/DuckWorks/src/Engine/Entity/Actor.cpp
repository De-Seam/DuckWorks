#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Actor)

Json Actor::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void Actor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

void Actor::Init(const Entity::InitParams& inInitParams)
{
	Base::Init(inInitParams);

	AddComponent<TransformComponent>();
}

void Actor::SetTransform(const fm::Transform2D& transform) 
{
	GetTransformComponent()->mTransform = transform; 
}

void Actor::SetPosition(const fm::vec2& inPosition) 
{
	GetTransformComponent()->mTransform.position = inPosition; 
}

void Actor::SetHalfSize(const fm::vec2& inHalfSize) 
{
	GetTransformComponent()->mTransform.halfSize = inHalfSize; 
}

void Actor::SetRotation(float inRotation) 
{
	GetTransformComponent()->mTransform.rotation = inRotation; 
}

MutexReadProtectedPtr<TransformComponent> Actor::GetTransformComponent() 
{ 
	return GetComponent<TransformComponent>(); 
}

fm::Transform2D Actor::GetTransform() 
{ 
	return GetTransformComponent()->mTransform; 
}

fm::vec2 Actor::GetPosition() 
{ 
	return GetTransform().position; 
}

fm::vec2 Actor::GetHalfSize() 
{ 
	return GetTransform().halfSize; 
}

float Actor::GetRotation()
{
	return GetTransform().rotation;
}
