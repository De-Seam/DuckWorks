#include <Precomp.h>
#include <Engine/Collision/CollisionNode.h>

// Engine includes
#include <Engine/World/World.h>
#include <Engine/Node/NodeMessages.h>

Json CollisionNode::Serialize() const
{
	Json json = Node::Serialize();

	JSON_SAVE(json, mRectangleHalfSize);

	return json;
}

void CollisionNode::Deserialize(const Json& inJson)
{
	Node::Deserialize(inJson);

	JSON_LOAD(inJson, mRectangleHalfSize);

	if (mCollisionShape)
	{
		Rectangle rectangle = GetRectangle();
		mCollisionShape->SetRectangle(rectangle);
		Transform2D transform = GetWorldTransform();
		transform.mPosition = rectangle.mPosition;
		transform.mRotation = rectangle.mRotation;
		SetWorldTransform(transform);
	}
}

CollisionNode::CollisionNode()
{
	gAssert(mCollisionShape == nullptr);
}

void CollisionNode::OnAddedToParent()
{
	Node::OnAddedToParent();

	mCollisionShape = new CollisionShape(GetRectangle(), GetWorld()->GetGrid());
	GetParent()->RegisterMessageListener(this, &CollisionNode::OnPreTransformUpdate);
	this->RegisterMessageListener(this, &CollisionNode::OnPreTransformUpdate);
}

void CollisionNode::OnRemovedFromParent()
{
	GetParent()->UnregisterMessageListener(this, &CollisionNode::OnPreTransformUpdate);
	this->UnregisterMessageListener(this, &CollisionNode::OnPreTransformUpdate);
	delete mCollisionShape;

	Node::OnRemovedFromParent();
}

void CollisionNode::OnTransformUpdated()
{
	Node::OnTransformUpdated();

	mCollisionShape->SetPosition(GetWorldTransform().mPosition);
}

void CollisionNode::OnPreTransformUpdate(MsgPreTransformUpdate& ioMessage)
{
	Rectangle rectangle;
	rectangle.mPosition = ioMessage.mNewWorldTransform.mPosition;
	rectangle.mHalfSize = mRectangleHalfSize * ioMessage.mNewWorldTransform.mScale;
	rectangle.mRotation = ioMessage.mNewWorldTransform.mRotation;

	mCollisionShape->SetRectangle(rectangle);

	ioMessage.mNewWorldTransform.mPosition = rectangle.mPosition;
	ioMessage.mNewWorldTransform.mRotation = rectangle.mRotation;
}

Rectangle CollisionNode::GetRectangle() const
{
	Rectangle rectangle;
	rectangle.mPosition = GetWorldTransform().mPosition;
	rectangle.mHalfSize = mRectangleHalfSize * GetWorldTransform().mScale;
	rectangle.mRotation = GetWorldTransform().mRotation;
	return rectangle;
}