#include <Precomp.h>
#include <Engine/Node/Node.h>

void Node::Render()
{
	for (const Ref<Node>& child : mChildren)
	{
		child->Render();
	}
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