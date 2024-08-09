#pragma once
#include <Core/RTTI/RTTIRefObject.h>

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

	void SetParent(const Ref<Node>& inParent) { gAssert(mParent == nullptr || inParent == nullptr); mParent = inParent; }
	const Node* GetParent() const { return mParent; }

	void AddChild(const Ref<Node>& inChild);
	void RemoveChild(Node& inChild);
	const Array<Ref<Node>>& GetChildren() const { return mChildren; }

protected:
	virtual void OnAddedToWorld();
	virtual void OnRemovedFromWorld();
	virtual void OnTransformUpdated() {}

private:
	Transform2D mWorldTransform;
	Transform2D mLocalTransform;

	Ref<Node> mParent;
	Array<Ref<Node>> mChildren;

	IF_DEBUG(bool mAddedToWorld = false;)
};
