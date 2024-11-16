#pragma once
// Engine includes
#include <Engine/Resources/Resource.h>

// External includes
#include <External/SDL/SDL_render.h>

class TextureResource : public Resource
{
	RTTI_CLASS(TextureResource, Resource)
public:
	virtual void Load() override;
	virtual void Unload() override;

private:
	SDL_Texture* mTexture = nullptr;
};
