#include <Precomp.h>
#include <Engine/Collision/CollisionNode.h>

// Engine includes
#include <Engine/World/World.h>

Json CollisionNode::Serialize() const
{
	Json json = Node::Serialize();

	json["HalfSize"] = mCollisionShape->GetHalfSize();

	return json;
}

void CollisionNode::Deserialize(const Json& inJson)
{
	Node::Deserialize(inJson);

	if (inJson.contains("HalfSize"))
		mCollisionShape->SetHalfSize(inJson["HalfSize"].get<Vec2>());
}

CollisionNode::CollisionNode()
{
	gAssert(mCollisionShape == nullptr);
}

void CollisionNode::OnAddedToParent()
{
	Node::OnAddedToParent();

	const Transform2D& transform = GetWorldTransform();
	Rectangle rectangle;
	rectangle.mPosition = transform.mPosition;
	rectangle.mHalfSize = Vec2(64, 64);
	rectangle.mRotation = transform.mRotation;
	mCollisionShape = new CollisionShape(rectangle, GetWorld()->GetGrid());
}

void CollisionNode::OnRemovedFromParent()
{
	delete mCollisionShape;

	Node::OnRemovedFromParent();
}

void CollisionNode::OnTransformUpdated()
{
	Node::OnTransformUpdated();

	mCollisionShape->SetPosition(GetWorldTransform().mPosition);
}
