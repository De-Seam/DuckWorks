#include <DuckCore/Containers/Array.h>

#include <Engine/Resources/ResourceManager.h>

void ResourceManager::UnloadUnreferencedResources()
{
	mResources.RemoveIf([](const DC::String&, Resource* inResource)
	{
		if (inResource->GetRefCount() == 1)
		{
			inResource->Unload();
			return true;
		}
		return false;
	});
}
