#pragma once
// Engine includes
#include <Engine/Entity/Components/WorldComponent.h>
#include <Engine/Resources/TextureResource.h>

// External includes
#include <External/SFML/Graphics/Sprite.hpp>

namespace sf {
class RenderWindow;
}

class TextureRenderComponent : public WorldComponent
{
	RTTI_CLASS(TextureRenderComponent, WorldComponent)

public:
	void SetTexture(TextureResource* inTexture);
	TextureResource* GetTexture() { return mTexture; }
	const TextureResource* GetTexture() const { return mTexture; }

	// Sets the transform of the sprite. Only to be used when needed, as it recalculates the transform
	void SetTransform(const Transform2D& inTransform);

	const sf::Sprite& GetSprite() const { return mSprite; }

private:
	Ref<TextureResource> mTexture = nullptr;
	sf::Sprite mSprite;
};

