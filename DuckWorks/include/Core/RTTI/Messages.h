#pragma once
#include <DuckCore/RTTI/RTTIRefClass.h>
#include "Core/Allocators/StandardAllocator.h"
#include "Core/Utilities/Types.h"

class Entity;

class MsgBase : public DC::RTTIClass
{
	RTTI_CLASS(MsgBase, RTTIClass)
};

class MsgEntityBase : public MsgBase
{
	RTTI_CLASS(MsgEntityBase, MsgBase)

public:
	Entity* mEntity = nullptr;
};

// Entity Position
class MsgPreEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityPositionUpdated, MsgEntityBase)

public:
	Vec2 mNewPosition;
};

class MsgPostEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityPositionUpdated, MsgEntityBase)

public:
	Vec2 mOldPosition;
	Vec2 mNewPosition;
};

// Entity Rotation
class MsgPreEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityRotationUpdated, MsgEntityBase)

public:
	float mNewRotation = 0.0f;
};

class MsgPostEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityRotationUpdated, MsgEntityBase)

public:
	float mOldRotation = 0.0f;
	float mNewRotation = 0.0f;
};
