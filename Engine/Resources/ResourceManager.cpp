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
	const std::function<Resource*(const Json&)>* constructor_function_ptr = mResourceTypeNamesToConstructorFunctions.Find(class_name);
	gAssert(constructor_function_ptr != nullptr, *String::sFormatted("Resource class %s not registered.", *class_name));
	Resource* resource = (*constructor_function_ptr)(inJson);
	RegisterResource(*resource);
	return resource;
}

void ResourceManager::RegisterResourceType(const DC::RTTI& inRTTI, std::function<Resource*(const DC::Json&)> inConstructorFunction)
{
	gAssert(!mResourceTypeNamesToConstructorFunctions.Contains(inRTTI.GetClassName()));
	mResourceTypeNamesToConstructorFunctions[inRTTI.GetClassName()] = Move(inConstructorFunction);
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
	mResourceLinksFile = Managers::sGet<FileManager>().Get<JsonFile>(cResourceLinksPath);
	if (mResourceLinksFile->GetJson().is_null())
		return;
	mResourceLinks = mResourceLinksFile->GetJson().get<HashMap<GUID, ResourceLinkInfo>>();
}

void ResourceManager::RegisterResource(Resource& inResource)
{
	mResources[inResource.GetGUID()] = &inResource;
}

Json ResourceManager::ResourceLinkInfo::ToJson() const
{
	Json json;
	JSON_SAVE(json, mGUID);
	JSON_SAVE(json, mTypeName);
	JSON_SAVE(json, mResourceName);
	JSON_SAVE(json, mJsonFilePath);
	return json;
}

void ResourceManager::ResourceLinkInfo::FromJson(const Json& inJson)
{
	JSON_LOAD(inJson, mGUID);
	JSON_LOAD(inJson, mTypeName);
	JSON_LOAD(inJson, mResourceName);
	JSON_LOAD(inJson, mJsonFilePath);
}

void ResourceManager::GetResourceLinkInfosOfType(const String& inTypeName, Array<const ResourceLinkInfo*>& outResourceLinkInfos) const
{
	gAssert(outResourceLinkInfos.IsEmpty());
	mResourceLinks.ForEach([&outResourceLinkInfos, &inTypeName](const GUID& inGUID, const ResourceLinkInfo& inResourceLinkInfo)
	{
		if (inResourceLinkInfo.mTypeName == inTypeName)
			outResourceLinkInfos.Add(&inResourceLinkInfo);
	});
}
