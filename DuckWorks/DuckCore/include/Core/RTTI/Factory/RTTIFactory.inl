#pragma once

template<typename taType>
taType* RTTIFactory::NewInstance() 
{
	const RTTI* rtti = taType::sGetRTTI();
	return rtti->NewInstance();
}

template<typename taType>
taType* RTTIFactory::NewInstance(const String& inClassName)
{
	const RTTI* rtti = GetRTTI(inClassName);
	RTTIClass* instantiated_class = rtti->NewInstance();
	gAssert(instantiated_class->IsA<taType>());
	return reinterpret_cast<taType*>(instantiated_class);
}

template<typename taType>
taType* RTTIFactory::NewInstance(const Json& inJson) 
{
	String class_name = inJson["ClassName"].get<std::string>();
	taType* instantiated_class = NewInstance<taType>(class_name);
	instantiated_class->Deserialize(inJson);
	return instantiated_class;
}

template<typename taType>
void RTTIFactory::RegisterClass() 
{
	const RTTI& rtti = taType::sGetRTTI();
	mClassNameToRTTI[rtti.GetClassName()] = &rtti;
}

template<typename taType>
void RTTIFactory::GetSubClassNames(Array<String>& outClassNames) const 
{
	GetSubClassNames(taType::sGetRTTI().GetClassName(), outClassNames);
}