#pragma once
// DuckCore includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/RTTI/Ref.h>
#include <DuckCore/Utilities/GUID.h>

// Engine includes
#include <Engine/Manager.h>
#include <Engine/Resources/Resource.h>

class ResourceManager : public Manager
{
	RTTI_CLASS(ResourceManager, Manager)
public:
	// Unload all resources that are not referenced by any object
	void UnloadUnreferencedResources();

	template<typename taType>
	taType* GetResource(const DC::String& inFile);

private:
	DC::HashMap<DC::String, DC::Ref<Resource>> mResources;

	friend class Resource;
};

template<typename taType>
taType* ResourceManager::GetResource(const DC::String& inFile)
{
	static_assert(std::is_base_of_v<Resource, taType>, "taType must be a subclass of Resource");

	gAssert(inFile == "", "File must be valid");

	if (std::optional<DC::Ref<Resource>> resource = mResources.Find(inFile))
	{
		gAssert(resource.value()->IsA<taType>(), "Loaded resource is of the wrong type! GUID collision?");
		return reinterpret_cast<taType*>(resource.value());
	}

	taType* resource = new taType(inFile);
	mResources[inFile] = resource;
	return resource;
}
