#pragma once
#include <Engine/Resources/Resource.h>

// External includes
#include <External/SFML/Graphics/Texture.hpp>

class TextureResource : public Resource
{
	RTTI_CLASS(TextureResource, Resource)
public:
	virtual ~TextureResource() override;

	virtual void Load(const String& inFile) override;
	virtual void Unload() override;

	const sf::Texture& GetTexture() const { return mTexture; }
private:
	sf::Texture mTexture;
};
