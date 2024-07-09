#pragma once
#include "Core/RTTI/RTTI.h"
#include "Core/Utilities/Types.h"

class MsgBase : public RTTIBaseClass
{
	RTTI_VIRTUAL_CLASS(MsgBase, RTTIBaseClass)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	MsgBase(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}
};
