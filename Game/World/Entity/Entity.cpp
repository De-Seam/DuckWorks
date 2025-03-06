#include <Game/World/Entity/Entity.h>

#include <Game/World/Entity/Components/Component.h>
#include <Game/World/Entity/Components/Components.h>
#include <Game/World/Scene/Scene.h>

using namespace DC;

Entity::Entity(Scene& aScene, GUID aGUID) :
	Base(aGUID),
	ECSEntity(aScene, aScene.GetRegistry().create())
{}

Entity::Entity(Scene& aScene, const Json& aJson) :
	Base(aJson),
	ECSEntity(aScene, aScene.GetRegistry().create())
{
	const Json& json_components = aJson["Components"];

	static thread_local Array<const RTTI*> sComponentRTTIs;
	sComponentRTTIs.Clear();
	GetAllComponentRTTIs(sComponentRTTIs);

	for (const RTTI* rtti : sComponentRTTIs)
	{
		auto it = json_components.find(rtti->GetClassName());
		if (it != json_components.end())
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
	Json json = Base::ToJson();
	json["ClassName"] = GetRTTI().GetClassName();

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

Entity::~Entity()
{
	GetScene().GetRegistry().destroy(GetEntityHandle());
}