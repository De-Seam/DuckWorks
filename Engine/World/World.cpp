#include <Engine/World/World.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Files/FileManager.h>

using namespace DC;

World::World(const GUID& aGUID) :
	Base(aGUID)
{
	// Default initialize the active Scene. We always need one active Scene.
	mActiveScene = new Scene;
}

World::World(const Json& aJson) :
	Base(aJson)
{
	mGUIDToSceneJson = aJson["mGUIDToSceneJson"];
	const GUID& active_scene_guid = aJson["ActiveSceneGUID"].get<GUID>();
	LoadScene(active_scene_guid);

	mActiveScene = mGUIDToInstantiatedScene[active_scene_guid];
}

Json World::ToJson() const
{
	Json json = Base::ToJson();
	json["ActiveSceneGUID"] = mActiveScene->GetGUID();
	json["mGUIDToSceneJson"] = mGUIDToSceneJson;
	return json;
}

void World::Update(float aDeltaTime)
{
	mActiveScene->Update(aDeltaTime);
}

void World::Render()
{
	mActiveScene->Render();	
}

void World::LoadScene(const GUID& aSceneGUID)
{
	if(mGUIDToInstantiatedScene.Contains(aSceneGUID))
	{
		gAssert(false, "Scene already loaded.");
		return;
	}

	HashMap<GUID, Ref<JsonFile>>::Iterator iter = mGUIDToSceneJson.Find(aSceneGUID);
	if (!iter.IsValid())
	{
		gAssert(false, "Scene GUID was not found in mGUIDToSceneJson list.");
		return;
	}

	JsonFile& json_file = *iter.GetValue();
	json_file.Load();
	Json& json = json_file.GetJson();

	Scene* scene = new Scene(json);
	gAssert(scene->GetGUID() == aSceneGUID);
	mGUIDToInstantiatedScene[aSceneGUID] = scene;
	json_file.ClearCachedContents();
}

void World::UnloadScene(const GUID& aSceneGUID)
{
	if (mActiveScene->GetGUID() == aSceneGUID)
	{
		gAssert(false, "Cannot unload the active Scene.");
		return;
	}
	gVerify(mGUIDToInstantiatedScene.Remove(aSceneGUID));
}
