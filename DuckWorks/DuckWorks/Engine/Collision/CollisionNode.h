#pragma once
// Engine includes
#include <Engine/Collision/CollisionShape.h>
#include <Engine/Node/Node.h>

struct MsgPreTransformUpdate;

class CollisionNode : public Node
{
	RTTI_CLASS(CollisionNode, Node)
public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	CollisionNode();

protected:
	virtual void OnAddedToParent() override;
	virtual void OnRemovedFromParent() override;

	virtual void OnTransformUpdated() override;

	// Message listeners
	void OnPreTransformUpdate(MsgPreTransformUpdate& ioMessage);

private:
	Rectangle GetRectangle() const;

	CollisionShape* mCollisionShape = nullptr;
	Vec2 mRectangleHalfSize = {64.0f, 64.0f};
};