#pragma once
#include <Core/Utilities/TypeID.h>

using MsgTypeID = TypeID<struct MsgBase>;

struct MsgBase
{
	STATIC_TYPE_ID(Msg)
};
