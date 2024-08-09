#pragma once
#include <Core/RTTI/RTTIRefObject.h>

class Resource : public RTTIRefObject
{
	RTTI_VIRTUAL_CLASS(Resource, RTTIRefObject)
public:
	virtual void Load(const String& inFile) = 0;
	virtual void Unload() = 0;

	const String& GetFile() const { return mFile; }

private:
	String mFile;
};