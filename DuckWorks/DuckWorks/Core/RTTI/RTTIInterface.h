#pragma once
#include <Core/Utilities/TypeID.h>

using RTTIInterfaceTypeID = TypeID<class RTTIInterface>;

#define RTTI_INTERFACE() \
public: \
	inline static RTTIInterfaceTypeID sInterfaceTypeID; \
private:

class RTTIInterface
{
	RTTI_INTERFACE()
public:

};