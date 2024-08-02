#include <Engine/Precomp.h>
#include <Engine/Resources/ResourceManager.h>

void ResourceManager::Shutdown()
{
	Manager::Shutdown();

	for (Pair<const String, Ref<Resource>>& resource : mResources)
	{
		resource.second->Unload();
	}
}
