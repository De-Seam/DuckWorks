#include <Engine/Resources/ResourceManager.h>

#include <DuckCore/Containers/Array.h>

void ResourceManager::UnloadUnreferencedResources()
{
	mResources.RemoveIf([](const DC::String&, const Resource* inResource)
	{
		return inResource->GetRefCount() == 1;
	});
}
