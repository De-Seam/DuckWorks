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

	Node();
	virtual ~Node() override;
	virtual void Render();

	void SetWorldPosition(const Vec2 &inPosition) { SetWorldTransform(Transform2D(inPosition, mWorldTransform.mScale, mWorldTransform.mRotation)); }
	void SetLocalPosition(const Vec2 &inPosition) { SetLocalTransform(Transform2D(inPosition, mLocalTransform.mScale, mLocalTransform.mRotation)); }
	const Vec2& GetWorldPosition() const { return mWorldTransform.mPosition; }
	const Vec2& GetLocalPosition() const { return mLocalTransform.mPosition; }

	void SetWorldScale(const Vec2 &inScale) { SetWorldTransform(Transform2D(mWorldTransform.mPosition, inScale, mWorldTransform.mRotation)); }
	void SetLocalScale(const Vec2 &inScale) { SetLocalTransform(Transform2D(mLocalTransform.mPosition, inScale, mLocalTransform.mRotation)); }
	const Vec2& GetWorldScale() const { return mWorldTransform.mScale; }
	const Vec2& GetLocalScale() const { return mLocalTransform.mScale; }

	void SetWorldRotation(float inRotation) { SetWorldTransform(Transform2D(mWorldTransform.mPosition, mWorldTransform.mScale, inRotation)); }
	void SetLocalRotation(float inRotation) { SetLocalTransform(Transform2D(mLocalTransform.mPosition, mLocalTransform.mScale, inRotation)); }
	float GetWorldRotation() const { return mWorldTransform.mRotation; }
	float GetLocalRotation() const { return mLocalTransform.mRotation; }

	void SetWorldTransform(const Transform2D& inTransform);
	void SetLocalTransform(const Transform2D& inTransform);
	const Transform2D& GetWorldTransform() const { return mWorldTransform; }
	const Transform2D& GetLocalTransform() const { return mLocalTransform; }

	void SetParent(Node* inParent);
	Node* GetParent() { return mParent; }
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

	virtual void OnTransformUpdated();

private:
	Transform2D mWorldTransform;
	Transform2D mLocalTransform;

	Node* mParent;
	Array<Ref<Node>> mChildren;

	World* mWorld = nullptr;

	friend class World;
	friend class RootNode;
};
