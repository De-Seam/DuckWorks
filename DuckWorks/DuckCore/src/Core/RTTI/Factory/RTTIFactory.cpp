#include <Core/Precomp.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Core includes
#include <Core/Utilities/Assert.h>

RTTIFactory gRTTIFactory;

const RTTI* RTTIFactory::GetRTTI(const String& inClassName) const
{
	HashMap<String, const RTTI*>::const_iterator iterator = mClassNameToRTTI.find(inClassName);
	const bool found = iterator != mClassNameToRTTI.end();
	gAssert(!found && "Class not registered!");
	return found ? iterator->second : nullptr;
}
