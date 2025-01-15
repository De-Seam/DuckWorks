#pragma once
// Engine includes
#include <Engine/Renderer/Sprite.h>
#include <Game/Entity/Components/Component.h>

struct SpriteRenderComponent : public ComponentBase
{
	RTTI_CLASS(SpriteRenderComponent, ComponentBase)
public:
	DC::Ref<Sprite> mSprite;

	virtual void OnCreatedByEditor() override;
};
