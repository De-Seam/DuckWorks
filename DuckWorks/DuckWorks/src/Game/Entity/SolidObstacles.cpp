#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
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
	AddComponent<TextureRenderComponent>(texture_render_component_parameters);
}

void SolidObstacle::Init()
{
	Base::Init();
}
