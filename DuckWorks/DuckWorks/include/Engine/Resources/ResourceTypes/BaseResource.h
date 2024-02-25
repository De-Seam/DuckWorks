#pragma once
#include "Core/Utilities/RTTI.h"

struct BaseResource : public RTTIBaseStruct
{
	RTTI_STRUCT(BaseResource, RTTIBaseStruct)
	// Make sure to create a virtual destructor to avoid memory leaks
	virtual ~BaseResource() {};

	virtual void LoadFromFile(const String& inFile) = 0;
};
