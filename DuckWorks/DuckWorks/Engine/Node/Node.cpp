#include <Precomp.h>
#include <Engine/Node/Node.h>

Node::~Node() 
{
	gAssert(mWorld == nullptr); // We should have been removed from the world before being destroyed.
	for (Node* child : mChildren)
		child->SetParent(nullptr);
}

void Node::Render()
{
	for (const Ref<Node>& child : mChildren)
		child->Render();
}

void Node::SetWorldTransform(const Transform2D& inTransform) 
{
	mWorldTransform = inTransform;

	if (mParent != nullptr)
		mLocalTransform = mParent->mWorldTransform.GetInverse() * mWorldTransform;
	else
		mLocalTransform = mWorldTransform;

	for (const Ref<Node>& child : mChildren)
		child->SetWorldTransform(child->mWorldTransform);

	OnTransformUpdated();
}

void Node::SetLocalTransform(const Transform2D& inTransform) 
{
	mLocalTransform = inTransform;

	if (mParent != nullptr)
		mWorldTransform = mParent->mWorldTransform * mLocalTransform;
	else
		mWorldTransform = mLocalTransform;

	for (const Ref<Node>& child : mChildren)
		child->SetLocalTransform(child->mLocalTransform);

	OnTransformUpdated();
}

void Node::AddChild(const Ref<Node>& inChild)
{
	inChild->SetParent(this);
	mChildren.push_back(inChild);
}

void Node::RemoveChild(Node& inChild)
{
	inChild.SetParent(nullptr);
	for (uint64 i = 0; i < mChildren.size(); i++)
	{
		if (mChildren[i].Get() == &inChild)
		{
			mChildren.erase(mChildren.begin() + i);
			return;
		}
	}
}

void Node::OnAddedToWorld(World* inWorld) 
{
	mWorld = inWorld;
}

void Node::OnRemovedFromWorld(World* inWorld) 
{
	gAssert(mWorld == inWorld);
	mWorld = nullptr;
}
