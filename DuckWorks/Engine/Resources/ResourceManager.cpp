#include <Engine/Resources/ResourceManager.h>

#include <DuckCore/Containers/Array.h>
#include <Engine/Engine.h>
#include <Engine/Files/FileManager.h>

using namespace DC;

ResourceManager::ResourceManager()
{
	ReadResourceLinksFromFile();
}

ResourceManager::~ResourceManager()
{
	WriteResourceLinksToFile();
}

Resource* ResourceManager::CreateResource(const Json& inJson)
{
	const String& class_name = inJson["ClassName"];
	const std::function<Resource*(const Json&)>* constructor_function_ptr = mResourceNamesToConstructorFunctions.Find(class_name);
	gAssert(constructor_function_ptr != nullptr, String::sFormatted("Resource class %s not registered.", *class_name));
	Resource* resource = (*constructor_function_ptr)(inJson);
	RegisterResource(*resource);
	return resource;
}

void ResourceManager::RegisterResourceType(const DC::RTTI& inRTTI, std::function<Resource*(const DC::Json&)> inConstructorFunction)
{
	gAssert(!mResourceNamesToConstructorFunctions.Contains(inRTTI.GetClassName()));
	mResourceNamesToConstructorFunctions[inRTTI.GetClassName()] = gMove(inConstructorFunction);
}

void ResourceManager::UnloadUnreferencedResources()
{
	mResources.RemoveIf([](const GUID&, const Resource* inResource)
	{
		return inResource->GetRefCount() == 1;
	});
}

void ResourceManager::LoadResourcesFromJsonFile(const JsonFile& inJsonFile)
{
	const auto resources = inJsonFile.GetJson().find("Resources");

	if (resources == inJsonFile.GetJson().end())
		return;

	for (const Json& resource : resources.value())
	{
		const GUID& guid = resource["mGUID"];

		// Skip resource if it's already loaded.
		if (mResources.Contains(guid))
			continue;

		CreateResource(resource);
	}
}

void ResourceManager::WriteResourceLinksToFile()
{
	mResourceLinksFile->GetJson() = Json(mResourceLinks);
	mResourceLinksFile->WriteToDisk();
}

void ResourceManager::ReadResourceLinksFromFile()
{
	mResourceLinksFile = gEngine->GetManager<FileManager>().Get<JsonFile>(cResourceLinksPath);
	mResourceLinks = mResourceLinksFile->GetJson().get<HashMap<GUID, String>>();
}

void ResourceManager::RegisterResource(Resource& inResource)
{
	mResources[inResource.GetGUID()] = &inResource;
}
