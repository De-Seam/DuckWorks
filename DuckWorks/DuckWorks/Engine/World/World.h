#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Engine includes
#include <Engine/Node/Node.h>

// External includes
#include <External/entt/entity/registry.hpp>

class Entity;

class World : public RTTIClass
{
	RTTI_CLASS(World, RTTIClass)

public:
	World();
	~World();

	void Update(float inDeltaTime);
	void Render();

	void AddNode(const Ref<Node>& inNode);
	void RemoveNode(const Node& inNode);

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

protected:
	void OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle);

private:
	bool mIsUpdatingEntities = false;

	Array<Ref<Node>> mNodes;

	entt::registry mRegistry;
};