#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Events/EventManager.h>

#include <Engine/Events/Events.h>
#include <Engine/Objects/Object.h>
#include <Engine/Resources/JsonFile.h>
#include <Engine/World/ServiceHolder.h>
#include <Engine/World/Scene/Scene.h>

class World : public Object, public ServiceHolder<WorldService>
{
	RTTI_CLASS(World, Object)
public:

	explicit World(const DC::Json& aJson);
	virtual DC::Json ToJson() const override;

	static DC::Ref<World> sNew(); // Create a new World.

	void Update(float aDeltaTime);
	void Render();

	void LoadScene(const DC::GUID& aSceneGUID);
	void UnloadScene(const DC::GUID& aSceneGUID);

	Scene& GetActiveScene() { return *mActiveScene; }

private:
	explicit World(const DC::GUID& aGUID); // Create a new World.

	DC::Ref<Scene> mActiveScene; // The currently active, instantiated Scene.
	DC::HashMap<DC::GUID, DC::Ref<Scene>> mGUIDToInstantiatedScene; // Maps each instantiated Scene to its GUID.
	DC::HashMap<DC::GUID, DC::Ref<JsonFile>> mGUIDToSceneJson; // Maps each Scene GUID to its serialized json data file.
};
