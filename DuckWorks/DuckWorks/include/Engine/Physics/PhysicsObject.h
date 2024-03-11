#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

class PhysicsObject : public RTTIBaseClass
{
	RTTI_CLASS(PhysicsObject, RTTIBaseClass)

	struct InitParams
	{
		fm::vec2 mLocation;
		fm::vec2 mHalfSize;
	};
	PhysicsObject() = default;

private:
	fm::vec2 mLocation;
	fm::vec2 mHalfSize;
};