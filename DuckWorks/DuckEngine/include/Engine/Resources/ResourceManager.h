#pragma once
// Engine includes
#include <Engine/Manager/Manager.h>
#include <Engine/Resources/Resource.h>

class ResourceManager : public Manager
{
	RTTI_MANAGER(ResourceManager, Manager)
public:
	template<typename taType>
	Ref<taType> Get(String inFile);

private:
	HashMap<String, Ref<Resource>> mResources;
};

template<typename taType>
Ref<taType> ResourceManager::Get(String inFile) 
{
	Resource* resource = mResources[inFile];
	if (resource != nullptr)
	{
		gAssert(resource->IsA(taType::sGetRTTI()) && "Resource was already loaded, but as a different type!");
		return resource;
	}

	resource = new taType;
	resource->Load(inFile);
	return resource;
}
