#pragma once
// Core includes
#include <Core/RTTI/Message.h>
#include <Core/Math/Transform.h>

class Node;

struct MsgNode : public MsgBase
{
	RTTI_MSG()

	Node* mNode = nullptr;
};

struct MsgPreTransformUpdate : public MsgNode
{
	RTTI_MSG()

	Transform2D mNewWorldTransform;
};

struct MsgPostTransformUpdate : public MsgNode
{
	RTTI_MSG()

	Transform2D mOldWorldTransform;
	Transform2D mOldLocalTransform;
};
