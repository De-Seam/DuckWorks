#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"

// External includes
#include "External/SDL/SDL_image.h"

RTTI_CLASS_DECLARATION(TextureResource)

Json TextureResource::Serialize() const
{
	// We don't want to serialize the texture or its size, since that might change if the texture changes
	return Base::Serialize();
}

void TextureResource::Deserialize(const Json& inJson)
{
	if (inJson["mFile"] != GetFileName())
	{
		gLog(LogType::Error, "TextureResource::Deserialize should not be called with a different file than the current file!");
		return;
	}

	Base::Deserialize(inJson);
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

void TextureResource::ReloadTexture()
{
	DestroyTexture();
	LoadFromFile(GetFileName());
}

void TextureResource::DestroyTexture()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}
