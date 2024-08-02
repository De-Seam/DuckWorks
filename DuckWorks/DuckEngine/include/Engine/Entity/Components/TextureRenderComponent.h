#pragma once
// Engine includes
#include <Engine/Entity/Components/WorldComponent.h>
#include <Engine/Resources/TextureResource.h>

// External includes
#include <External/SFML/Graphics/RectangleShape.hpp>

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

	void SetSize(const Vec2& inSize);
	const sf::RectangleShape& GetRectangle() const { return mRectangle; }

private:
	Ref<TextureResource> mTexture = nullptr;
	sf::RectangleShape mRectangle;
};

