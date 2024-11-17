#include <Engine/Renderer/TextureResource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>

// External includes
#include <External/SDL/SDL_image.h>
#include <External/SDL/SDL_render.h>

TextureResource::TextureResource(const DC::String& inFile) :
	Resource(inFile)
{
	SDL_Renderer* renderer = gEngine->GetManager<Renderer>().GetRenderer();
	gAssert(renderer != nullptr, "Can't create texture resources before creating the renderer");
	mTexture = IMG_LoadTexture(renderer, *GetFile());
}

TextureResource::~TextureResource()
{
	gAssert(mTexture != nullptr);

	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
}
