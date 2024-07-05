#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
#include <Engine/Entity/Components/CollisionComponent.h>
#include "Engine/Entity/Components.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(SolidObstacle, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(SolidObstacle)

SolidObstacle::SolidObstacle(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	TextureRenderComponent::ConstructParameters texture_render_component_parameters;
	String texture_path = "Assets/DefaultTexture.png";
	texture_render_component_parameters.mTexture = gResourceManager.GetResource<TextureResource>(texture_path);
	texture_render_component_parameters.mHalfSize = Vec2{32.f, 32.f};
	AddComponent<TextureRenderComponent>(texture_render_component_parameters);

	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mHalfSize = Vec2{32.f, 32.f};
	AddComponent<CollisionComponent>(collision_component_params);
}

void SolidObstacle::Init()
{
	Base::Init();
}
