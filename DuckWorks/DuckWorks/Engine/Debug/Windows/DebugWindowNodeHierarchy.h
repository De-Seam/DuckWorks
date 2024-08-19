#pragma once
#include <Engine/Debug/Windows/DebugWindow.h>

class Node;

class DebugWindowNodeHierarchy : public DebugWindow
{
	RTTI_CLASS(DebugWindowNodeHierarchy, DebugWindow)
public:
	virtual void Update(float inDeltaTime) override;

private:
	void RecursiveDrawNode(Node& inNode, int inDepth);

	WeakRef<Node> mSelectedNode = nullptr;

	HashMap<GUID, bool> mWasCollapsedMap;
};