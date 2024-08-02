#include <Engine/Precomp.h>
#include <Engine/Entity/Components/TransformComponent.h>

// External includes
#include <External/entt/entt.hpp>

void TransformComponent::SetTransform(const Transform2D& inTransform, entt::registry& inRegistry, entt::entity inEntityHandle) 
{
	mTransform = inTransform;
	inRegistry.emplace_or_replace<TransformUpdatedTag>(inEntityHandle);
}
