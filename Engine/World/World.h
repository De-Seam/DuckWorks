#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/HashMap.h>

#include <Engine/Objects/Object.h>
#include <Engine/Resources/JsonFile.h>
#include <Engine/World/Scene/Scene.h>

class World : public Object
{
	RTTI_CLASS(World, Object)
public:
	World();

	void LoadScene(const DC::GUID& aSceneGUID);
	void UnloadScene(const DC::GUID& aSceneGUID);

	Scene& GetActiveScene() { return *mActiveScene; }

private:
	DC::Ref<Scene> mActiveScene; // The currently active, instantiated Scene.
	DC::HashMap<DC::GUID, DC::Ref<Scene>> mGUIDToInstantiatedScene; // Maps each instantiated Scene to its GUID.
	DC::HashMap<DC::GUID, DC::Ref<JsonFile>> mGUIDToSceneJson; // Maps each Scene GUID to its serialized json data file.
};
