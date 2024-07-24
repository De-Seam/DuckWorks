#include <Core/Precomp.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Core includes
#include <Core/Utilities/Assert.h>

RTTIFactory gRTTIFactory;

const RTTI* RTTIFactory::GetRTTI(const String& inClassName) const
{
	const RTTI* const* rtti_ptr = mClassNameToRTTI.Find(inClassName);
	gAssert(rtti_ptr != nullptr && "Class not registered!");
	return rtti_ptr != nullptr ? *rtti_ptr : nullptr;
}
