#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"

// External includes
#include "External/SDL/SDL_image.h"

Json TextureResource::Serialize() const
{
	// We don't want to serialize the texture or its size, since that might change if the texture changes
	return Base::Serialize();
}

void TextureResource::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

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
		return;
	}

	SDL_QueryTexture(mTexture, nullptr, nullptr, &mSize.x, &mSize.y);
}
