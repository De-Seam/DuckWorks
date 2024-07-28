#include <Core/Precomp.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Core includes
#include <Core/Utilities/Assert.h>

const RTTI* RTTIFactory::GetRTTI(const String& inClassName) const
{
	if (mClassNameToRTTI.contains(inClassName))
		return mClassNameToRTTI.at(inClassName);
	return nullptr;
	//HashMap<String, const RTTI*>::const_iterator iterator = mClassNameToRTTI.find(inClassName);
	//const bool found = iterator != mClassNameToRTTI.end();
	//gAssert(!found && "Class not registered!");
	//return found ? iterator->second : nullptr;
}

void RTTIFactory::GetClassNames(Array<String>& outClassNames) const 
{
	for (const Pair<const String, const RTTI*>& pair : mClassNameToRTTI)
		outClassNames.push_back(pair.first);
}

void RTTIFactory::GetSubClassNames(const String& inClassName, Array<String>& outClassNames) const 
{
	const RTTI* target_rtti = GetRTTI(inClassName);
	for (const Pair<const String, const RTTI*>& pair : mClassNameToRTTI)
	{
		if (IsSubClassRecursive(pair.second, target_rtti))
			outClassNames.push_back(pair.first);
	}
}

bool RTTIFactory::IsSubClassRecursive(const RTTI* inBaseRTTI, const RTTI* inTargetRTTI) const 
{
	const RTTI* base_rtti = GetRTTI(inBaseRTTI->GetBaseClassName());
	if (base_rtti == nullptr)
		return false;
	if (base_rtti->GetTypeID() == inTargetRTTI->GetTypeID())
		return true;
	return IsSubClassRecursive(base_rtti, inTargetRTTI);
}
