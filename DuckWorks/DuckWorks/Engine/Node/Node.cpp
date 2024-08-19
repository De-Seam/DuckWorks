#include <Precomp.h>
#include <Engine/Node/Node.h>

// Engine includes
#include <Engine/Node/NodeMessages.h>

Json Node::Serialize() const
{
	Json json = RTTIRefObject::Serialize();

	JSON_SAVE(json, mLocalTransform);
	JSON_SAVE(json, mChildren);

	return json;
}

void Node::Deserialize(const Json& inJson)
{
	RTTIRefObject::Deserialize(inJson);

	gAssert(GetParent() != nullptr || mWorld != nullptr); // We should have a parent before deserializing (or we're the root node and already have a world)

	ClearChildren();

	for (const Json& child_json : inJson["mChildren"])
	{
		Ref<Node> child = gCoreModule->mRTTIFactory.NewInstance<Node>(String(child_json["ClassName"].get<std::string>()));
		AddChild(child);
		child->Deserialize(child_json);
	}
	JSON_LOAD(inJson, mLocalTransform);
	SetLocalTransform(mLocalTransform);
}

Node::~Node() 
{
	gAssert(mWorld == nullptr); // We should have been removed from the world before being destroyed.
	ClearChildren();
}

void Node::Render()
{
	for (const Ref<Node>& child : mChildren)
		child->Render();
}

void Node::SetWorldTransform(const Transform2D& inTransform) 
{
	// Can't set transform without a parent
	gAssert(mParent != nullptr);

	MsgPreTransformUpdate pre_msg;
	pre_msg.mNode = this;
	pre_msg.mNewWorldTransform = inTransform;

	BroadcastMessage(pre_msg);

	MsgPostTransformUpdate post_msg;
	post_msg.mNode = this;
	post_msg.mOldWorldTransform = mWorldTransform;
	post_msg.mOldLocalTransform = mLocalTransform;

	mWorldTransform = pre_msg.mNewWorldTransform;
	Transform2D local_transform = mParent->mWorldTransform.GetInverse() * mWorldTransform;
	mLocalTransform = local_transform;

	BroadcastMessage(post_msg);

	OnTransformUpdated();
}

void Node::SetLocalTransform(const Transform2D& inTransform) 
{
	// Can't set transform without a parent
	gAssert(mParent != nullptr);

	Transform2D world_transform = mParent->mWorldTransform * inTransform;
	SetWorldTransform(world_transform);
}

void Node::SetParent(Node* inParent) 
{
	gAssert(mParent == nullptr || inParent == nullptr);
	if (inParent == nullptr)
	{
		OnRemovedFromParent();
		mParent = inParent;
	}
	else
	{
		mParent = inParent;
		OnAddedToParent();
	}
}

void Node::AddChild(const Ref<Node>& inChild)
{
	gAssert(inChild->GetParent() == nullptr);
	gAssert(inChild.Get() != this);
	inChild->SetParent(this);
	mChildren.push_back(inChild);
}

void Node::RemoveChild(const Node& inChild)
{
	for (uint64 i = 0; i < mChildren.size(); i++)
	{
		if (mChildren[i].Get() == &inChild)
		{
			mChildren.erase(mChildren.begin() + i);
			return;
		}
	}
}

void Node::ClearChildren() 
{
	for (Node* child : mChildren)
		child->SetParent(nullptr);
	mChildren.clear();
}

void Node::OnAddedToParent() 
{
	gAssert(mWorld == nullptr);
	mWorld = mParent->GetWorld();
	gAssert(mWorld != nullptr);
}

void Node::OnRemovedFromParent() 
{
	gAssert(mWorld != nullptr);
	mWorld = nullptr;
}

void Node::OnTransformUpdated()
{
	for (const Ref<Node>& child : mChildren)
		child->SetLocalTransform(child->GetLocalTransform());
}
