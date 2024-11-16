#include <Engine/Resources/Resource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

Resource* Resource::sFindResourceInResourceManagerInternal(const DC::GUID& inGUID)
{
	return gEngine->GetManager<ObjectManager>().Find<Resource>(inGUID);
}
