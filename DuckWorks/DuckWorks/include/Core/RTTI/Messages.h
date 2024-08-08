#pragma once
#include <Core/Utilities/TypeID.h>

using MsgTypeID = TypeID<struct MsgBase>;

#define RTTI_MSG() STATIC_TYPE_ID(Msg)

struct MsgBase
{
	RTTI_MSG()
};