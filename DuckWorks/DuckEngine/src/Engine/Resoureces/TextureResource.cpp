#include <Engine/Precomp.h>
#include <Engine/Resources/TextureResource.h>

// External includes
#include <External/SFML/Graphics/Texture.hpp>

TextureResource::~TextureResource() 
{
	gAssert(mTexture.getSize() == sf::Vector2u(0, 0), "Texture was not unloaded!");
}

void TextureResource::Load(const String& inFile) 
{
	mTexture.loadFromFile(*inFile);
}

void TextureResource::Unload() 
{
	mTexture = sf::Texture();
}
