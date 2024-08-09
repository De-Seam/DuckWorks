#pragma once
// Core includes
#include <Core/Containers/HashMap.h>
#include <Core/Containers/String.h>
#include <Core/RTTI/RTTI.h>

class RTTIFactory
{
public:
	RTTIClass* NewInstance(const String& inClassName);
	template<typename taType>
	taType* NewInstance();
	template<typename taType>
	taType* NewInstance(const String& inClassName);
	template<typename taType>
	taType* NewInstance(const Json& inJson);

	const RTTI* GetRTTI(const String& inClassName) const;
	const RTTI* FindRTTI(const String& inClassName) const;

	template<typename taType>
	void RegisterClass();

	void GetClassNames(Array<String>& outClassNames) const; ///< Get all registered class names. Very slow operation.

	template<typename taType>
	void GetSubClassNames(Array<String>& outClassNames) const; ///< Get all registered class names that are subclasses of taType. Very slow operation.
	void GetSubClassNames(const String& inClassName, Array<String>& outClassNames) const; ///< Get all registered class names that are subclasses of inClassName. Very slow operation.

private:
	void RegisterRTTI(const RTTI& inRTTI);
	bool IsSubClassRecursive(const RTTI* inBaseRTTI, const RTTI* inTargetRTTI) const;

	HashMap<String, const RTTI*> mClassNameToRTTI;
};

#include "RTTIFactory.inl"
