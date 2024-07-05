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

class MsgPreEntityTransformUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityTransformUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPreEntityTransformUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Entity* mEntity = nullptr;
	Transform2D mNewTransform;
};

class MsgPostEntityTransformUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityTransformUpdated, MsgEntityBase, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPostEntityTransformUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Entity* mEntity = nullptr;
	Transform2D mOldTransform;
	Transform2D mNewTransform;
};
