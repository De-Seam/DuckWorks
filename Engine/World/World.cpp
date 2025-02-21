#include <Engine/World/World.h>

using namespace DC;

World::World()
{
	// Default initialize the active Scene. We always need one active Scene.
	mActiveScene = new Scene;
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

	mGUIDToInstantiatedScene[aSceneGUID] = new Scene(aSceneGUID, json);
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
