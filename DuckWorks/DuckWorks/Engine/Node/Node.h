#pragma once
// Core includes
#include <Core/RTTI/RTTIRefObject.h>

class World;

class Node : public RTTIRefObject
{
	RTTI_CLASS(Node, RTTIRefObject)
public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	virtual ~Node() override;
	virtual void Render();

	void SetWorldTransform(const Transform2D& inTransform);
	void SetLocalTransform(const Transform2D& inTransform);
	void UpdateWorldTransform();

	const Transform2D& GetWorldTransform() const { return mWorldTransform; }
	const Transform2D& GetLocalTransform() const { return mLocalTransform; }

	void SetParent(Node* inParent);
	const Node* GetParent() const { return mParent; }

	void AddChild(const Ref<Node>& inChild);
	void RemoveChild(const Node& inChild);
	const Array<Ref<Node>>& GetChildren() const { return mChildren; }
	void ClearChildren();

	World* GetWorld() { return mWorld; }
	const World* GetWorld() const { return mWorld; }

protected:
	virtual void OnAddedToParent();
	virtual void OnRemovedFromParent();

	virtual void OnTransformUpdated() {}

private:
	Transform2D mWorldTransform;
	Transform2D mLocalTransform;

	Node* mParent;
	Array<Ref<Node>> mChildren;

	World* mWorld = nullptr;

	friend class World;
	friend class RootNode;
};
