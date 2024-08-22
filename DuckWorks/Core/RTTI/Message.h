#pragma once
#include <Core/Utilities/TypeID.h>

DECLARE_TYPE_ID(MsgTypeID)

#define RTTI_MSG() STATIC_TYPE_ID(Msg)

struct MsgBase
{
	RTTI_MSG()
};
