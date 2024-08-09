#pragma once
#include <Core/RTTI/RTTIRefObject.h>

class Node : public RTTIRefObject
{
	RTTI_CLASS(Node, RTTIRefObject)
public:
	virtual void Render();

	void SetParent(const Ref<Node>& inParent) { gAssert(mParent == nullptr || inParent == nullptr); mParent = inParent; }
	const Node* GetParent() const { return mParent; }

	void AddChild(const Ref<Node>& inChild);
	void RemoveChild(Node& inChild);
	const Array<Ref<Node>>& GetChildren() const { return mChildren; }

private:
	Ref<Node> mParent;
	Array<Ref<Node>> mChildren;
};
