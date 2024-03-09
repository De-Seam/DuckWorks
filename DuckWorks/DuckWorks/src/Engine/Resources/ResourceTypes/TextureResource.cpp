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
	// If we deserialize a texture, then all other resources that reference this texture will be changed as well.
	// And after this calling gResourceManager.GetResource<TextureResource>(mFile) will return the wrong texture, unless this file is the same.
	gLog(LogType::Error, "TextureResource::Deserialize should not be called!");

	Base::Deserialize(inJson);

	DestroyTexture();

	LoadFromFile(GetFileName());
}

TextureResource::~TextureResource()
{
	DestroyTexture();
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

void TextureResource::DestroyTexture()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}
