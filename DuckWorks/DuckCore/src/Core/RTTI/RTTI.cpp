#include <Core/Precomp.h>
#include <Core/RTTI/RTTI.h>

RTTI::RTTI(
	const char* inClassName, 
	const char* inBaseClassName, 
	std::function<RTTIClass* ()> inConstructorFunction)
	:
	mClassName(inClassName),
	mBaseClassName(inBaseClassName),
	mConstructorFunction(inConstructorFunction) 
{}
