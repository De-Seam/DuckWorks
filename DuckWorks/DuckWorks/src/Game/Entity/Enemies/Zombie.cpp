#include "Precomp.h"
#include "Game/Entity/Enemies/Zombie.h"

// Engine includes
#include <Engine/Entity/Components.h>
#include <Engine/Resources/ResourceManager.h>

RTTI_CLASS_DEFINITION(Zombie, ClassAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(Zombie)

Zombie::Zombie(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	TextureRenderComponent::ConstructParameters params;
	params.mTexture = gResourceManager.GetResource<TextureResource>("Assets/TinySwords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png");
	params.mTransform.mHalfSize = Vec2{64.f, 64.f};
	params.mUseSrcRect = true;
	params.mSrcRect = {0, 0, 192, 192};
	AddComponent<TextureRenderComponent>(params);
}
