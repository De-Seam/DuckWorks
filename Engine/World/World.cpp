#include <Engine/World/World.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Files/FileManager.h>

using namespace DC;

World::World(const Json& aJson) {}

Json World::ToJson()
{
	Json json;
	json["ActiveSceneGUID"] = mActiveScene->GetGUID();
	json["Scenes"] = mGUIDToSceneJson;
	return json;
}

Ref<World> World::sNew()
{
	return new World(GUID::sCreate());
}

void World::LoadScene(const GUID& aSceneGUID)
{
	gAssert(!mGUIDToInstantiatedScene.Contains(aSceneGUID), "Scene is already loaded.");

	Ref<JsonFile>* json_file_ptr = mGUIDToSceneJson.Find(aSceneGUID);
	if (json_file_ptr == nullptr)
	{
		gAssert(false, "Invalid Scene GUID.");
		return;
	}

	JsonFile& json_file = *json_file_ptr->Get();
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

World::World(const GUID& aGUID) :
	Base(aGUID)
{
	// Default initialize the active Scene. We always need one active Scene.
	mActiveScene = new Scene;
}
