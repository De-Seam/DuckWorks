#include <Engine/Renderer/TextureResource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/SurfaceFile.h>
#include <Engine/Files/FileManager.h>

// External includes
#include <External/SDL/SDL_image.h>
#include <External/SDL/SDL_render.h>

using namespace DC;

TextureResource::TextureResource(const Json& inJson) :
	Resource(inJson)
{
	SDL_Renderer* renderer = gEngine->GetManager<Renderer>().GetRenderer();
	gAssert(renderer != nullptr, "Can't create texture resources before creating the renderer");

	mSurfaceFilePath = inJson["mSurfaceFilePath"];

	Ref<SurfaceFile> surface_file = gEngine->GetManager<FileManager>().Get<SurfaceFile>(mSurfaceFilePath);
	if (surface_file == nullptr)
	{
		gLog(LogLevel::Error, String::sFormatted("Failed to load surface file \"%s\" for texture resource.", *mSurfaceFilePath));
		return;
	}

	mTexture = SDL_CreateTextureFromSurface(renderer, surface_file->GetSurface());
}

TextureResource::~TextureResource()
{
	SDL_DestroyTexture(mTexture);
	mTexture = nullptr;
}

Json TextureResource::ToJson() const
{
	Json json = Resource::ToJson();
	json["mSurfaceFilePath"] = mSurfaceFilePath;
	return json;
}
