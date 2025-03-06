#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Events/EventManager.h>

#include <Engine/Events/Events.h>
#include <Engine/Objects/Object.h>
#include <Engine/Resources/JsonFile.h>

#include <Game/World/Service.h>
#include <Game/World/ServiceHolder.h>
#include <Game/World/Scene/Scene.h>

class World : public Object, public ServiceHolder<WorldService>
{
	RTTI_CLASS(World, Object)
public:
	explicit World(const DC::GUID& aGUID); // Create a new empty World.
	explicit World(const DC::Json& aJson); // Load a World from a Json object.
	virtual DC::Json ToJson() const override;

	void Update(float aDeltaTime);
	void Render();

	void LoadScene(const DC::GUID& aSceneGUID);
	void UnloadScene(const DC::GUID& aSceneGUID);

	Scene& GetActiveScene() { return *mActiveScene; }

private:

	DC::Ref<Scene> mActiveScene; // The currently active, instantiated Scene.
	DC::HashMap<DC::GUID, DC::Ref<Scene>> mGUIDToInstantiatedScene; // Maps each instantiated Scene to its GUID.
	DC::HashMap<DC::GUID, DC::Ref<JsonFile>> mGUIDToSceneJson; // Maps each Scene GUID to its serialized json data file.
};
