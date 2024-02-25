#include "Precomp.h"
#include "Engine/Resources/ResourceManager.h"

ResourceManager gResourceManager;

ResourceManager::ResourceManager()
{
	assert(this == &gResourceManager);
}
