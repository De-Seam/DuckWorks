#pragma once
#include <DuckCore/RTTI/RTTIRefClass.h>
#include <DuckCore/Utilities/GUID.h>

class Object : public DC::RTTIRefClass
{
	RTTI_CLASS(Object, RTTIRefClass)
public:
	explicit Object(const DC::GUID& aGUID = DC::GUID::sNone());
	explicit Object(const DC::Json& aJson);
	virtual ~Object() override;
	virtual DC::Json ToJson() const;

	void SetGUID(const DC::GUID& aGUID) { mGUID = aGUID; }
	DC::GUID GetGUID() const { return mGUID; }


private:
	DC::GUID mGUID;
};
