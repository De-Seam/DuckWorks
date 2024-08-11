#pragma once
#include <Engine/Node/Node.h>

class World;

class RootNode : public Node
{
	RTTI_CLASS(RootNode, Node)
public:
	RootNode() { gAssert(false); }
	RootNode(World& inWorld) { mWorld = &inWorld; }
	virtual ~RootNode() override { mWorld = nullptr; }
};