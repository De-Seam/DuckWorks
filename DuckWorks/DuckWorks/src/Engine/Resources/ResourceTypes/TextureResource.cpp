#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

#include "Engine/Renderer/Renderer.h"
#include "External/SDL/SDL_image.h"

TextureResource::~TextureResource()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}

void TextureResource::LoadFromFile(const String& inFile)
{
	Base::LoadFromFile(inFile);

	mTexture = IMG_LoadTexture(gRenderer.GetRenderer(), inFile.c_str());
	if (mTexture == nullptr)
	{
		gLog("Failed to load texture: %s", inFile.c_str());
	}
}
