#include <Engine/World/Entity/Entity.h>

#include <Engine/World/Entity/Components/Component.h>
#include <Engine/World/Entity/Components/Components.h>
#include <Engine/World/Scene/Scene.h>

using namespace DC;

void Entity::OnAddedToScene(Scene& aScene)
{
	gAssert(mScene == nullptr, "Entity is already in a Scene.");
	mScene = &aScene;
}

void Entity::OnRemovedFromScene(const Scene& aScene)
{
	gAssert(mScene == &aScene, "Entity is not part of this Scene.");
	mScene = nullptr;
}

void Entity::FromJson(const Json& aJson)
{
	const Json& json_components = aJson["Components"];

	static thread_local Array<const RTTI*> sComponentRTTIs;
	sComponentRTTIs.Clear();
	GetAllComponentRTTIs(sComponentRTTIs);

	for (const RTTI* rtti : sComponentRTTIs)
	{
		auto it = json_components.find(rtti->GetClassName());
		if (*it)
		{
			Component* component = ::FindComponent(*this, *rtti);
			if (component == nullptr)
				component = ::AddComponent(*this, *rtti);

			if (component != nullptr)
				component->FromJson(it.value());
		}
	}
}

Json Entity::ToJson() const
{
	Json json;
	Json& json_components = json["Components"];

	static thread_local Array<const RTTI*> sComponentRTTIs;
	sComponentRTTIs.Clear();
	GetAllComponentRTTIs(sComponentRTTIs);

	for (const RTTI* rtti : sComponentRTTIs)
	{
		if (const Component* component = ::FindComponent(*this, *rtti))
			json_components[rtti->GetClassName()] = component->ToJson();
	}

	return json;
}
