#pragma once
// Engine includes
#include <Engine/Collision/CollisionShape.h>
#include <Engine/Node/Node.h>

class CollisionNode : public Node
{
	RTTI_CLASS(CollisionNode, Node)
public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

protected:
	virtual void OnAddedToParent() override;
	virtual void OnRemovedFromParent() override;

	virtual void OnTransformUpdated() override;

private:
	CollisionShape* mCollisionShape = nullptr;
};