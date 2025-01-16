#pragma once
// DuckCore includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/RTTI/Ref.h>
#include <DuckCore/Utilities/GUID.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Manager.h>
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
class ResourceManager : public Manager
{
	RTTI_CLASS(ResourceManager, Manager)
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

private:
	DC::HashMap<DC::GUID, DC::Ref<Resource>> mResources; // All resources that are currently loaded
	DC::HashMap<DC::GUID, DC::String> mResourceLinks; //Links GUIDs of resources to the file path they can be found in
	DC::Ref<JsonFile> mResourceLinksFile; // The file that contains the resource links

	DC::HashMap<DC::String, std::function<Resource*(const DC::Json&)>> mResourceNamesToConstructorFunctions;

	friend class Resource;
};

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

	const DC::String* json_path_ptr = mResourceLinks.Find(inGUID);
	if (json_path_ptr == nullptr)
		return nullptr; // Resource could not be found in our resource links.

	DC::Ref<const JsonFile> json_file = gEngine->GetManager<FileManager>().Get<JsonFile>(*json_path_ptr);
	LoadResourcesFromJsonFile(*json_file);

	DC::Ref<Resource>* resource_ptr = mResources.Find(inGUID);
	if (resource_ptr == nullptr)
	{
		gAssert(false);
		gLog(DC::LogLevel::Error, DC::String::sFormatted("Resource %s was not found in file %s.", *inGUID.ToString(), **json_path_ptr));
		return nullptr;
	}
	return &resource_ptr->Get()->Cast<taType>();
}

template<typename taResourceType>
inline void from_json(const DC::Json& inJson, DC::Ref<taResourceType>& outResource)
{
	const DC::GUID& guid = inJson["mGUID"];
	outResource = gEngine->GetManager<ResourceManager>().Find<taResourceType>(guid);
}
