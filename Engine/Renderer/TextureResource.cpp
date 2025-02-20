#include <Engine/Renderer/TextureResource.h>

#include <DuckCore/Manager/Managers.h>

#include <Engine/Engine.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/SurfaceFile.h>

#include <External/SDL/SDL_image.h>
#include <External/SDL/SDL_render.h>

using namespace DC;

TextureResource::TextureResource(const Json& inJson) :
	Resource(inJson)
{
	SDL_Renderer* renderer = Managers::sGet<Renderer>().GetRenderer();
	gAssert(renderer != nullptr, "Can't create texture resources before creating the renderer");

	mSurfaceFilePath = inJson["mSurfaceFilePath"];

	Ref<SurfaceFile> surface_file = Managers::sGet<FileManager>().Get<SurfaceFile>(mSurfaceFilePath);
	if (surface_file == nullptr)
	{
		Log(ELogLevel::Error, String::sFormatted("Failed to load surface file \"%s\" for texture resource.", *mSurfaceFilePath));
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
