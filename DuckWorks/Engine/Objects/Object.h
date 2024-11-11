#pragma once
#include <DuckCore/RTTI/RTTIRefClass.h>
#include <DuckCore/Utilities/GUID.h>

class Object : public DC::RTTIRefClass
{
	RTTI_CLASS(Object, RTTIRefClass)
public:
	Object();
	virtual ~Object() override;

	DC::GUID GetGUID() const { return mGUID; }

private:
	DC::GUID mGUID = DC::GUID::sCreate();
};
