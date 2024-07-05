#pragma once
#include "Core/Allocators/StandardAllocator.h"
#include "Core/RTTI/RTTI.h"
#include "Core/Utilities/Types.h"

class Entity;

class MsgBase : public RTTIBaseClass
{
	RTTI_VIRTUAL_CLASS(MsgBase, RTTIBaseClass)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgBase(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}
};

class MsgEntityBase : public MsgBase
{
	RTTI_VIRTUAL_CLASS(MsgEntityBase, MsgBase)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgEntityBase(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Entity* mEntity = nullptr;
};

// Entity Position
class MsgPreEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityPositionUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPreEntityPositionUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Vec2 mNewPosition;
};

class MsgPostEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityPositionUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPostEntityPositionUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Vec2 mOldPosition;
	Vec2 mNewPosition;
};

// Entity Rotation
class MsgPreEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityRotationUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPreEntityRotationUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	float mNewRotation = 0.0f;
};

class MsgPostEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityRotationUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPostEntityRotationUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	float mOldRotation = 0.0f;
	float mNewRotation = 0.0f;
};
