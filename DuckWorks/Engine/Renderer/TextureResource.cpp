#include <Engine/Renderer/TextureResource.h>

// External includes
#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>

#include <External/SDL/SDL_image.h>
#include <External/SDL/SDL_render.h>

void TextureResource::Load()
{
	SDL_Renderer* renderer = gEngine->GetManager<Renderer>().GetRenderer();
	gAssert(renderer != nullptr, "Can't create texture resources before creating the renderer");
	mTexture = IMG_LoadTexture(renderer, *GetFile());
}

void TextureResource::Unload()
{
	gAssert(mTexture != nullptr);

	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
}
