#include "Precomp.h"
#include "Engine/Resources/ResourceManager.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

ResourceManager gResourceManager;

ResourceManager::ResourceManager()
{
	assert(this == &gResourceManager);
}

void ResourceManager::Init()
{
	// We save this for caching
	mDefaultTexture = GetResource<TextureResource>("Assets/DefaultTexture.png");
}
