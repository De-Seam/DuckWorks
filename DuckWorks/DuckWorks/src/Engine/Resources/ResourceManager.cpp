#include "Precomp.h"
#include "Engine/Resources/ResourceManager.h"

// Engine includes
#include "Engine/Engine/Engine.h"
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

void ResourceManager::Update()
{
#ifdef _DEBUG

	for (auto it = mResources.begin(); it != mResources.end(); ++it)
	{
		const String& file = it->first; // Reference to the key
		BaseResource* resource = it->second.get(); // Reference to the resource

		int64 time = GetTimeSinceFileModified(file);

		if (time != resource->mFileLastModifiedTime)
		{
			// Reload the resource
			resource->Reload();
			resource->mFileLastModifiedTime = time;
		}
	}

#endif // _DEBUG
}

int64 ResourceManager::GetTimeSinceFileModified(const String& inFile) const
{
	if (!gEngine.FileExists(inFile.c_str()))
		return -1;
	std::filesystem::file_time_type time_stamp = std::filesystem::last_write_time(inFile);
	return time_stamp.time_since_epoch().count();
}
