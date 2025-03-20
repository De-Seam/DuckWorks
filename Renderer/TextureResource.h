/*
#pragma once
// Engine includes
#include <Engine/Resources/Resource.h>

struct SDL_Texture;

class TextureResource : public Resource
{
	RTTI_CLASS(TextureResource, Resource)
public:
	TextureResource(const DC::Json& inJson);
	virtual ~TextureResource() override;

	virtual DC::Json ToJson() const override;

	SDL_Texture* GetTexture() const { return mTexture; }

private:
	DC::String mSurfaceFilePath;
	SDL_Texture* mTexture = nullptr;
};
*/