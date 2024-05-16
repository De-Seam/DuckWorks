#pragma once
// Own includes
#include "ResourceTypes/BaseResource.h"

// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Log/Log.h"

// Std includes
#include <filesystem>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager() = default;

	//Delete copy constructor
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	template<typename taType>
	std::shared_ptr<taType> GetResource(const String& inFile);

private:
	phmap::flat_hash_map<std::string, SharedPtr<BaseResource>> mResources;

private:
};

extern ResourceManager gResourceManager;

template<typename taType>
std::shared_ptr<taType> ResourceManager::GetResource(const String& inFile)
{
	// taType must be derived from BaseResource
	static_assert(std::is_base_of_v<BaseResource, taType>);
	if (!std::filesystem::exists(inFile))
	{
		gLog(ELogType::Error, "File doesn't exist: %s", inFile.c_str());
		return nullptr;
	}

	if (!mResources.contains(inFile))
	{
		// Load resource if it isn't already loaded
		gLog("Loading resource of type %s: %s", taType::sGetClassName(), inFile.c_str());
		mResources[inFile] = std::make_shared<taType>();
		mResources[inFile]->LoadFromFile(inFile);
	}

	return std::static_pointer_cast<taType>(mResources[inFile]);
}
