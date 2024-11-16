#pragma once
#include <DuckCore/RTTI/RTTIRefClass.h>
#include <DuckCore/Utilities/GUID.h>

class Object : public DC::RTTIRefClass
{
	RTTI_CLASS(Object, RTTIRefClass)
public:
	Object(const DC::GUID& inGUID = DC::GUID::sCreate());
	virtual ~Object() override;

	void SetGUID(const DC::GUID& inGUID);
	DC::GUID GetGUID() const { return mGUID; }

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

private:
	DC::GUID mGUID;;
};
