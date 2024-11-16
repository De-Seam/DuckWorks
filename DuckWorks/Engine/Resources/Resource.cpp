#include <Engine/Resources/Resource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Resources/ResourceManager.h>

Resource* Resource::sGetResourceInResourceManager(const DC::GUID& inGUID)
{
	return gEngine->GetManager<ResourceManager>().GetResource<Resource>(inGUID);
}

Resource* FileResource::sGetResourceInResourceManager(const DC::String& inFile)
{
	return gEngine->GetManager<ResourceManager>().GetFileResource<FileResource>(inFile);
}
