#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/Pair.h>
#include <DuckCore/Core/Log.h>
#include <DuckCore/Manager/Manager.h>
#include <DuckCore/Manager/Managers.h>
#include <DuckCore/RTTI/Ref.h>
#include <DuckCore/Utilities/GUID.h>

#include <Engine/Engine.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Resources/JsonFile.h>
#include <Engine/Resources/Resource.h>

/**

Assets/Textures/ExampleTexture.png
Assets/Saves/Save.json

Assets/Resources/EntityResources.json:
    - EntityResource:
        - GUID: XXXX-XXXX
		- Name: "Player"
        - Components:
            - SpriteRenderComponent:
                - SpriteResource: XXXX-XXXX
            - AnimationComponent:
                - TextureResource: XXXX-XXXX
                -  Animations:
                    - Idle: {0, 0, 32, 32}, {32, 0, 32, 32}, {64, 0, 32, 32}
					- Moving: {0, 32, 32, 32}, {32, 32, 32, 32}, {64, 32, 32, 32}

Assets/Resources/TextureResourcesA.json:
Resources:
     - TextureResource:
        - GUID: XXXX-XXXX
		- Name: "ExampleTexture"
		- File: Assets/Textures/ExampleTexture.png
    - TextureResource:
        - GUID: XXXX-XXXX
		- Name: "PlayerTexture"
		- File: Assets/Texture/Player.png

Assets/Resources/TextureResourcesB.json:
    - TextureResource:
        - GUID: XXXX-XXXX
		- Name: "Trees"
		- File: Assets/Texture/Trees.png

Assets/RandomResources.json:
    Resources
    {
    - SpriteResource:
        - GUID: XXXX-XXXX
		- Name: "Tree"
		- TextureResource: XXXX-XXXX
    - SaveResources:
        - GUID: XXXX-XXXX
		- Name: "GameSave"
		- File: Assets/Saves/Save.json
    }

Assets/Resources/ResourceLinks.json:	// Basically one big hash map
    - XXXX-XXXX: Assets/RandomResources.json
	- XXXX-XXXX: Assets/RandomResources.json
	- XXXX-XXXX: Assets/TextureResourcesA.json
	- XXXX-XXXX: Assets/TextureResourcesA.json
	- XXXX-XXXX: Assets/TextureResourcesA.json
	- XXXX-XXXX: Assets/TextureResourcesB.json
	- XXXX-XXXX: Assets/TextureResourcesB.json

Assets/ResourceLinks.json:
[
    ["XXXX-XXXX", "Assets/TextureResourcesA.json"],
    ["XXXX-XXXX", "Assets/TextureResourcesA.json"]
]

Potential for future:
Cooked ResourceLinks, converts it to binary format.

**/
class ResourceManager : public DC::Manager
{
	MANAGER_BASE_CLASS(ResourceManager)
public:
	constexpr static const char* cResourceLinksPath = "Assets/ResourceLinks.json";

	ResourceManager();
	virtual ~ResourceManager() override;

	Resource* CreateResource(const DC::Json& inJson); // Create a resource from a json object.
	template<typename taType>
	void RegisterResourceType(); // Register a resource type.
	void RegisterResourceType(const DC::RTTI& inRTTI, std::function<Resource*(const DC::Json&)> inConstructorFunction);

	// Unload all resources that are not referenced by any object
	void UnloadUnreferencedResources();

	void LoadResourcesFromJsonFile(const JsonFile& inJsonFile); // Load all resources contained in the provided json.

	void WriteResourceLinksToFile();
	void ReadResourceLinksFromFile();

	void RegisterResource(Resource& inResource);

	template<typename taType>
	taType& Get(const DC::GUID& inGUID);

	template<typename taType>
	taType* Find(const DC::GUID& inGUID);

	struct ResourceLinkInfo
	{
		DC::GUID mGUID; // The GUID of the resource.
		DC::String mTypeName; // The type name of the resource. E.g. "TextureResource"
		DC::String mResourceName; // The user-given name of the resource. E.g. "Player_Texture"
		DC::String mJsonFilePath; // The path to the Json file that contains the resource. E.g. "Assets/Resources/PlayerResources.json"

		DC::Json ToJson() const;
		void FromJson(const DC::Json& inJson);
	};
	void GetResourceLinkInfosOfType(const DC::String& inTypeName, DC::Array<const ResourceLinkInfo*>& outResourceLinkInfos) const;

private:
	DC::HashMap<DC::GUID, DC::Ref<Resource>> mResources; // All resources that are currently loaded
	DC::HashMap<DC::GUID, ResourceLinkInfo> mResourceLinks; //Links GUIDs of resources to their ResourceLinkInfos.
	DC::Ref<JsonFile> mResourceLinksFile; // The file that contains the resource links

	DC::HashMap<DC::String, std::function<Resource*(const DC::Json&)>> mResourceTypeNamesToConstructorFunctions;

	friend class Resource;
};

inline void to_json(DC::Json& outJson, const ResourceManager::ResourceLinkInfo& inVariable)
{
	outJson = inVariable.ToJson();
}

inline void from_json(const DC::Json& inJson, ResourceManager::ResourceLinkInfo& outVariable)
{
	outVariable.FromJson(inJson);
}

template <typename taType>
void ResourceManager::RegisterResourceType()
{
	RegisterResourceType(taType::sGetRTTI(), [](const DC::Json& inJson) { return new taType(inJson); });
}

template<typename taType>
taType& ResourceManager::Get(const DC::GUID& inGUID)
{
	static_assert(std::is_base_of_v<Resource, taType>, "taType must be a subclass of Resource");
	gAssert(inGUID.IsValid());

	taType* resource = Find<taType>(inGUID);
	gAssert(resource != nullptr);
	return *resource;
}

template <typename taType>
taType* ResourceManager::Find(const DC::GUID& inGUID)
{
	static_assert(std::is_base_of_v<Resource, taType>, "taType must be a subclass of Resource");
	gAssert(inGUID.IsValid());

	if (DC::Ref<Resource>* resource_ptr = mResources.Find(inGUID))
		return &(*resource_ptr)->Cast<taType>(); // Resource was already loaded, return that.

	const ResourceLinkInfo* resource_link_info = mResourceLinks.Find(inGUID);
	if (resource_link_info == nullptr)
		return nullptr; // Resource could not be found in our resource links.

	DC::Ref<const JsonFile> json_file = DC::Managers::sGet<FileManager>().Get<JsonFile>(resource_link_info->mJsonFilePath);
	LoadResourcesFromJsonFile(*json_file);

	DC::Ref<Resource>* resource_ptr = mResources.Find(inGUID);
	if (resource_ptr == nullptr)
	{
		gAssert(false);
		gLog(DC::ELogLevel::Error, DC::String::sFormatted("Resource %s was not found in file %s.", *inGUID.ToString(), *resource_link_info->mJsonFilePath));
		return nullptr;
	}
	return &resource_ptr->Get()->Cast<taType>();
}

template<typename taResourceType>
inline void from_json(const DC::Json& inJson, DC::Ref<taResourceType>& outResource)
{
	const DC::GUID& guid = inJson["mGUID"];
	outResource = DC::Managers::sGet<ResourceManager>().Find<taResourceType>(guid);
}
