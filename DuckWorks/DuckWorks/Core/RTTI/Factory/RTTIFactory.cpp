#include <Precomp.h>
#include <Core/RTTI/Factory/RTTIFactory.h>

// Core includes
#include <Core/Utilities/Assert.h>

const RTTI* RTTIFactory::GetRTTI(const String& inClassName) const
{
	return mClassNameToRTTI.at(inClassName);
}

const RTTI* RTTIFactory::FindRTTI(const String& inClassName) const 
{
	HashMap<String, const RTTI*>::const_iterator iterator = mClassNameToRTTI.find(inClassName);
	const bool found = iterator != mClassNameToRTTI.end();
	return found ? iterator->second : nullptr;
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

void RTTIFactory::RegisterRTTI(const RTTI& inRTTI) 
{
	gAssert(!mClassNameToRTTI.contains(inRTTI.GetClassName()));
	mClassNameToRTTI[inRTTI.GetClassName()] = &inRTTI;
}

bool RTTIFactory::IsSubClassRecursive(const RTTI* inBaseRTTI, const RTTI* inTargetRTTI) const 
{
	const RTTI* base_rtti = FindRTTI(inBaseRTTI->GetBaseClassName());
	if (base_rtti == nullptr)
		return false;
	if (base_rtti->GetTypeID() == inTargetRTTI->GetTypeID())
		return true;
	return IsSubClassRecursive(base_rtti, inTargetRTTI);
}
