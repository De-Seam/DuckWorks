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

	SDL_Texture* GetTexture() const { return mTexture; }

private:
	SDL_Texture* mTexture = nullptr;
};
