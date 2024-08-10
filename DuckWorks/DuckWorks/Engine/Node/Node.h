#pragma once
// Core includes
#include <Core/RTTI/RTTIRefObject.h>

class World;

class Node : public RTTIRefObject
{
	RTTI_CLASS(Node, RTTIRefObject)
public:
	virtual ~Node() override;
	virtual void Render();

	void SetWorldTransform(const Transform2D& inTransform);
	void SetLocalTransform(const Transform2D& inTransform);

	const Transform2D& GetWorldTransform() const { return mWorldTransform; }
	const Transform2D& GetLocalTransform() const { return mLocalTransform; }

	void SetParent(Node* inParent) { gAssert(mParent == nullptr || inParent == nullptr); mParent = inParent; }
	const Node* GetParent() const { return mParent; }

	void AddChild(const Ref<Node>& inChild);
	void RemoveChild(Node& inChild);
	const Array<Ref<Node>>& GetChildren() const { return mChildren; }

	World* GetWorld() { return mWorld; }
	const World* GetWorld() const { return mWorld; }

protected:
	virtual void OnAddedToWorld(World* inWorld);
	virtual void OnRemovedFromWorld(World* inWorld);
	virtual void OnTransformUpdated() {}

private:
	Transform2D mWorldTransform;
	Transform2D mLocalTransform;

	Node* mParent;
	Array<Ref<Node>> mChildren;

	World* mWorld = nullptr;

	friend class World;
};
