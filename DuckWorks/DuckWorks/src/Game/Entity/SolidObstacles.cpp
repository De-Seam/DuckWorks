#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
#include "Engine/World/World.h"
#include "Engine/Resources/ResourceManager.h"

RTTI_CLASS_DEFINITION(SolidObstacle)

RTTI_EMPTY_SERIALIZE_DEFINITION(SolidObstacle)

SolidObstacle::SolidObstacle(World* inWorld)
	: Actor(inWorld)
{
	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");

	CollisionComponent& collision_component = AddComponent<CollisionComponent>();

	CollisionObject::InitParams params;
	params.mTransform = GetTransform();
	params.mType = CollisionObject::EType::Static;
	collision_component.mCollisionObjectHandle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
}
