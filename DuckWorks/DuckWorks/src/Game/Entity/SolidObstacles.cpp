#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(SolidObstacle)

RTTI_EMPTY_SERIALIZE_DEFINITION(SolidObstacle)

void SolidObstacle::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);

	AddComponent<TextureRenderComponent>();
	MutexReadProtectedPtr<TextureRenderComponent> texture_render_component = GetComponent<TextureRenderComponent>();
	texture_render_component->mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}
