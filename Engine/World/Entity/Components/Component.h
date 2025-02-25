#pragma once
#include <DuckCore/RTTI/RTTIClass.h>
#include <DuckCore/Utilities/Json.h>

struct Component : public DC::RTTIClass
{
	RTTI_CLASS(Component, RTTIClass)
public:
	virtual DC::Json ToJson() const = 0;
	virtual void FromJson(const DC::Json& aJson) = 0;
};
