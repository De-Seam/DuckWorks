#pragma once
#include <DuckCore/Containers/File/File.h>

struct SDL_Surface;

class SurfaceFile : public DC::File
{
public:
	SurfaceFile(DC::String inPath) : File(DC::Move(inPath)) {}

	virtual void Load() override; // Loads mContents from mPath. This will clear potentially existing content.
	virtual void WriteToDisk() override; // Writes mContents to mPath. Child classes should override this to update mContents before calling this.

	SDL_Surface* GetSurface() { return mSurface; }
	const SDL_Surface* GetSurface() const { return mSurface; }

private:
	SDL_Surface* mSurface = nullptr;
};
