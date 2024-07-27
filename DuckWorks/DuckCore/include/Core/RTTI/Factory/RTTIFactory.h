#pragma once
// Core includes
#include <Core/Containers/HashMap.h>
#include <Core/Containers/String.h>
#include <Core/RTTI/RTTI.h>

class RTTIFactory
{
public:
	template<typename taType>
	taType* NewInstance()
	{
		const RTTI* rtti = taType::sGetRTTI();
		return rtti->NewInstance();
	}

	template<typename taType>
	taType* NewInstance(const String& inClassName)
	{
		const RTTI* rtti = GetRTTI(inClassName);
		RTTIClass* instantiated_class = rtti->NewInstance();
		gAssert(instantiated_class->IsA<taType>());
		return reinterpret_cast<taType*>(instantiated_class);
	}

	template<typename taType>
	taType* NewInstance(const Json& inJson)
	{
		String class_name = inJson["ClassName"].get<std::string>();
		taType* instantiated_class = NewInstance<taType>(class_name);
		instantiated_class->Deserialize(inJson);
		return instantiated_class;
	}

	const RTTI* GetRTTI(const String& inClassName) const;

	template<typename taType>
	void RegisterClass()
	{
		const RTTI& rtti = taType::sGetRTTI();
		mClassNameToRTTI[rtti.GetClassName()] = &rtti;
	}

private:
	HashMap<String, const RTTI*> mClassNameToRTTI;
};
