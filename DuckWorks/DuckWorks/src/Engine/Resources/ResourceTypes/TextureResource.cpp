#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"

// External includes
#include "External/SDL/SDL_image.h"
#include "External/SDL/SDL_Render.h"

RTTI_CLASS_DEFINITION(TextureResource, ClassAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(TextureResource)

TextureResource::~TextureResource()
{
	DestroyTexture();
}

void TextureResource::LoadFromFile(const String& inFile)
{
	Base::LoadFromFile(inFile);

	gAssert(mTexture == nullptr);
	mTexture = IMG_LoadTexture(gRenderer.GetRenderer(), inFile.c_str());
	if (mTexture == nullptr)
	{
		gLog("Failed to load texture: %s", inFile.c_str());
		return;
	}

	SDL_QueryTexture(mTexture, nullptr, nullptr, &mSize.mX, &mSize.mY);
}

void TextureResource::Reload()
{
	DestroyTexture();
	Base::Reload();
}

uint64 TextureResource::GetMemorySize() const
{
	return sizeof(TextureResource) + SCast<uint64>(mSize.mX * mSize.mY * 4);
}

void TextureResource::DestroyTexture()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}

void from_json(const Json& inJson, SharedPtr<TextureResource>& outVariable)
{
	if (inJson.empty())
		return;

	outVariable = gResourceManager.GetResource<TextureResource>(inJson["mFile"]);
	outVariable->Deserialize(inJson);
}
