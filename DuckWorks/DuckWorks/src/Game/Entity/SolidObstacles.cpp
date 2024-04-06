#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
#include "Engine/World/World.h"
#include "Engine/Resources/ResourceManager.h"

RTTI_CLASS_DEFINITION(SolidObstacle)

RTTI_EMPTY_SERIALIZE_DEFINITION(SolidObstacle)	

void SolidObstacle::Init(const Entity::InitParams& inInitParams)
{
	Base::Init(inInitParams);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}