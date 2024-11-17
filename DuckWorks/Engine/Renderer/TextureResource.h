#pragma once
// Engine includes
#include <Engine/Resources/Resource.h>

struct SDL_Texture;

class TextureResource : public Resource
{
	RTTI_CLASS(TextureResource, Resource)
public:
	TextureResource(const DC::String& inFile);
	virtual ~TextureResource() override;

private:
	SDL_Texture* mTexture = nullptr;
};
