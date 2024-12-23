#pragma once
#include <DuckCore/RTTI/RTTIRefClass.h>
#include <DuckCore/Utilities/GUID.h>

class Object : public DC::RTTIRefClass
{
	RTTI_CLASS(Object, RTTIRefClass)
public:
	Object(const DC::GUID& inGUID = DC::GUID::sNone());
	virtual ~Object() override;

	void SetGUID(const DC::GUID& inGUID) { mGUID = inGUID; }
	DC::GUID GetGUID() const { return mGUID; }

private:
	DC::GUID mGUID;
};
