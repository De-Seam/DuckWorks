#pragma once
#include "BaseResource.h"

struct SDL_Texture;

struct TextureResource : public BaseResource
{
	RTTI_STRUCT(TextureResource, BaseResource)
	virtual ~TextureResource() override;
	virtual void LoadFromFile(const String& inFile) override;

	SDL_Texture* mTexture = nullptr;
};
