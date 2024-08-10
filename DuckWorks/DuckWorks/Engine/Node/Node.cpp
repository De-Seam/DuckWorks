#include <Precomp.h>
#include <Engine/Node/Node.h>

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

	UpdateWorldTransform();

	for (const Ref<Node>& child : mChildren)
		child->UpdateWorldTransform();
}

void Node::UpdateWorldTransform() 
{
	if (mParent != nullptr)
		mWorldTransform = mParent->mWorldTransform * mLocalTransform;
	else
		mWorldTransform = mLocalTransform;

	OnTransformUpdated();
}

void Node::SetParent(Node* inParent) 
{
	gAssert(mParent == nullptr || inParent == nullptr);
	mParent = inParent;
	if (inParent == nullptr)
		OnRemovedFromParent();
	else
		OnAddedToParent();
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
