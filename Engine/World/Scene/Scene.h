#pragma once
#include <DuckCore/Containers/Array.h>

#include <Engine/Objects/Object.h>
#include <Engine/World/Entity/Entity.h>

#include <External/entt/entity/registry.hpp>

class Scene : public Object
{
	RTTI_CLASS(Scene, Object)
public:
	Scene(const DC::GUID& aGUID = DC::GUID::sCreate());
	Scene(const DC::GUID& aGUID, const DC::Json& aJson);

	void AddEntity(DC::Ref<Entity>& aEntity);
	void RemoveEntity(Entity& aEntity);

private:
	DC::Array<DC::Ref<Entity>> mEntities;
	entt::registry mRegistry;
};
