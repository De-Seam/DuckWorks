#include <Engine/Renderer/SurfaceFile.h>

#include <Engine/Engine.h>

#include <SDL/SDL_image.h>
#include <SDL/SDL_surface.h>

using namespace DC;

void SurfaceFile::Load()
{
	mSurface = IMG_Load(*GetPath());
}

void SurfaceFile::WriteToDisk()
{
	String file_extension = GetFileExtension();
	int result = -1;
	if (file_extension == "png")
		result = IMG_SavePNG(mSurface, *GetPath());
	else if (file_extension == "jpg")
		result = IMG_SaveJPG(mSurface, *GetPath(), 100);
	else if (file_extension == "bmp")
		result = SDL_SaveBMP(mSurface, *GetPath());
#ifdef _ASSERTS
	else
		gAssert(false, "Unsupported file extension");
#endif

	if (result != 0)
		Log(ELogLevel::Error, String::sFormatted("Failed to write surface to disk: %s. Error: %s", *GetPath(), SDL_GetError()));
}
