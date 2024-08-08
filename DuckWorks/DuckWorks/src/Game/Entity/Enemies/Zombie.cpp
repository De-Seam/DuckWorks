#include "Precomp.h"
#include "Game/Entity/Enemies/Zombie.h"

// Engine includes
#include <Engine/Entity/Components.h>
#include <Engine/Resources/ResourceManager.h>

Zombie::Zombie(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	TextureRenderComponent::ConstructParameters params;
	params.mTexture = gResourceManager.GetResource<TextureResource>("Assets/TinySwords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png");
	params.mLocalOffset.mHalfSize = Vec2{64.f, 64.f};
	params.mUseSrcRect = true;
	params.mSrcRect = {0, 0, 192, 192};
	AddComponent<TextureRenderComponent>(params);
}
