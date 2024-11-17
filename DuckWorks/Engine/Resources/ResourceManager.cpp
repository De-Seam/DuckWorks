#include <DuckCore/Containers/Array.h>

#include <Engine/Resources/ResourceManager.h>

void ResourceManager::UnloadUnreferencedResources()
{
	mResources.RemoveIf([](const DC::String&, const Resource* inResource)
	{
		return inResource->GetRefCount() == 1;
	});
}
