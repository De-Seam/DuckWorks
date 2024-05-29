#pragma once
#include "Core/RTTI/RTTI.h"
#include "Core/Utilities/Types.h"

class Entity;

struct MsgBase : public RTTIBaseClass
{
	RTTI_VIRTUAL_CLASS(MsgBase, RTTIBaseClass)

};

struct MsgPreEntityTransformUpdated : public MsgBase
{
	RTTI_CLASS(MsgPreEntityTransformUpdated, MsgBase, StandardAllocator)

	Entity* mEntity = nullptr;
	fm::Transform2D mOldTransform;
	fm::Transform2D mNewTransform;
};

struct MsgPostEntityTransformUpdated : public MsgBase
{
	RTTI_CLASS(MsgPostEntityTransformUpdated, MsgBase, StandardAllocator)

	Entity* mEntity = nullptr;
	fm::Transform2D mOldTransform;
	fm::Transform2D mNewTransform;
};