#pragma once
// Engine includes
#include <Engine/Manager/Manager.h>
#include <Engine/Resources/Resource.h>

class ResourceManager : public Manager
{
	RTTI_MANAGER(ResourceManager, Manager)
public:
	virtual void Shutdown() override;

	template<typename taType>
	Ref<taType> Get(String inFile);

private:
	HashMap<String, Ref<Resource>> mResources;
};

template<typename taType>
Ref<taType> ResourceManager::Get(String inFile) 
{
	PROFILE_SCOPE(ResourceManager::Get)

	Ref<Resource>& resource = mResources[inFile];
	if (resource != nullptr)
	{
		gAssert(resource->IsA(taType::sGetRTTI()) && "Resource was already loaded, but as a different type!");
		return static_cast<taType*>(resource.Get());
	}

	resource = new taType;
	resource->Load(inFile);
	return static_cast<taType*>(resource.Get());
}
