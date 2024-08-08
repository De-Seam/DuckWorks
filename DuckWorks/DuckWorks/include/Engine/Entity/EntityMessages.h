#pragma once
// Core includes
#include "Entity.h"

#include "Core/Allocators/StandardAllocator.h"
#include "Core/RTTI/Messages.h"

class Entity;

enum class Entity::EUpdateFrequency : uint8;

// Entity Base
class MsgEntityBase : public MsgBase
{
	RTTI_VIRTUAL_CLASS(MsgEntityBase, MsgBase)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		Entity* mEntity = nullptr;
	};

	MsgEntityBase(const ConstructParameters& inConstructParameters = {})
		: Base(inConstructParameters),
		mEntity(inConstructParameters.mEntity)
	{
		gAssert(mEntity != nullptr);
	}

	Entity* GetEntity() const { return mEntity; }

private:
	Entity* mEntity = nullptr;
};

class MsgEntityUpdateFrequencyChanged : public MsgEntityBase
{
	RTTI_CLASS(MsgEntityUpdateFrequencyChanged, MsgEntityBase)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		Entity::EUpdateFrequency mOldUpdateFrequency = Entity::EUpdateFrequency::None;
		Entity::EUpdateFrequency mNewUpdateFrequency = Entity::EUpdateFrequency::None;
	};

	MsgEntityUpdateFrequencyChanged(const ConstructParameters& inConstructParameters = {})
		: Base(inConstructParameters),
		mOldUpdateFrequency(inConstructParameters.mOldUpdateFrequency),
		mNewUpdateFrequency(inConstructParameters.mNewUpdateFrequency) {}

	Entity::EUpdateFrequency GetOldUpdateFrequency() const { return mOldUpdateFrequency; }
	Entity::EUpdateFrequency GetNewUpdateFrequency() const { return mNewUpdateFrequency; }

private:
	Entity::EUpdateFrequency mOldUpdateFrequency = Entity::EUpdateFrequency::None;
	Entity::EUpdateFrequency mNewUpdateFrequency = Entity::EUpdateFrequency::None;
};

// Entity Position
class MsgPreEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityPositionUpdated, MsgEntityBase)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPreEntityPositionUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Vec2 mNewPosition;
};

class MsgPostEntityPositionUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityPositionUpdated, MsgEntityBase)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPostEntityPositionUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	Vec2 mOldPosition;
	Vec2 mNewPosition;
};

// Entity Rotation
class MsgPreEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPreEntityRotationUpdated, MsgEntityBase)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPreEntityRotationUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	float mNewRotation = 0.0f;
};

class MsgPostEntityRotationUpdated : public MsgEntityBase
{
	RTTI_CLASS(MsgPostEntityRotationUpdated, MsgEntityBase)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgPostEntityRotationUpdated(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	float mOldRotation = 0.0f;
	float mNewRotation = 0.0f;
};
