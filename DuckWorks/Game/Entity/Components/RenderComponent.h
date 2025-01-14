#pragma once
// Engine includes
#include <Engine/Renderer/Sprite.h>

struct SpriteRenderComponent : public ComponentBase
{
	RTTI_CLASS(SpriteRenderComponent, ComponentBase)
public:
	DC::Ref<Sprite> mSprite;
};