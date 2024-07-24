#pragma once
// Core includes
#include <Core/Containers/HashMap.h>
#include <Core/Containers/String.h>
#include <Core/RTTI/RTTI.h>

class RTTIClass;

class RTTIFactory
{
public:
	template<typename taType>
	taType* NewInstance() 
	{
		const RTTI* rtti = taType::sGetRTTI();
		return rtti->NewInstance();
	}

	const RTTI* GetRTTI(String inClassName) const;

	template<typename taType>
	void RegisterClass()
	{
		const RTTI* rtti = taType::sGetRTTI();
		mClassNameToRTTI[rtti->GetClassName()] = rtti;
	}

private:
	HashMap<String, const RTTI*> mClassNameToRTTI;
};

extern RTTIFactory gRTTIFactory;