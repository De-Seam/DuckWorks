#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Engine includes
#include <Engine/World/RootNode.h>

// External includes
#include <External/entt/entity/registry.hpp>

class Entity;

class World : public RTTIClass
{
	RTTI_CLASS(World, RTTIClass)

public:
	World();
	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render();

	void AddNode(const Ref<Node>& inNode);
	void RemoveNode(const Node& inNode);
	Node& GetRootNode() { return *mRootNode; }
	const Node& GetRootNode() const { return *mRootNode; }

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

protected:
	void OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle);

private:
	bool mIsUpdatingEntities = false;

	Ref<RootNode> mRootNode; ///< Root Node

	entt::registry mRegistry;
};