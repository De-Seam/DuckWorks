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
	template<typename taType>
	taType* GetResource(const DC::GUID& inGUID = DC::GUID::sCreate());

	template<typename taType>
	taType* GetFileResource(const DC::String& inFile);

private:
	DC::HashMap<DC::GUID, DC::Ref<Resource>> mResources;

	friend class Resource;
};

template<typename taType>
taType* ResourceManager::GetResource(const DC::GUID& inGUID)
{
	static_assert(std::is_base_of_v<Resource, taType>, "taType must be a subclass of Resource");
	static_assert(!std::is_base_of_v<FileResource, taType>, "taType must not be a subclass of FileResource. Use GetFileResource instead");

	gAssert(inGUID.IsValid(), "GUID must be valid");

	if (DC::Ref<Resource>* resource_ptr = mResources.Find(inGUID))
	{
		gAssert((*resource_ptr)->IsA<taType>(), "Loaded resource is of the wrong type! GUID collision?");
		return reinterpret_cast<taType*>(resource_ptr->Get());
	}

	taType* resource = new taType(inGUID);
	mResources[inGUID] = resource;
	return resource;
}

template<typename taType>
taType* ResourceManager::GetFileResource(const DC::String& inFile)
{
	static_assert(std::is_base_of_v<FileResource, taType>, "taType must be a subclass of FileResource");

	gAssert(inFile == "", "File must be valid");

	DC::GUID guid = DC::GUID(inFile.Hash());

	if (DC::Ref<Resource>* resource_ptr = mResources.Find(guid))
	{
		gAssert((*resource_ptr)->IsA<taType>(), "Loaded resource is of the wrong type! GUID collision?");
		return reinterpret_cast<taType*>(resource_ptr->Get());
	}

	taType* resource = new taType(guid, inFile);
	mResources[guid] = resource;
	return resource;
}
