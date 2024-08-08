#pragma once
#include "Core/RTTI/RTTI.h"

struct BaseResource : public RTTIClass
{
	RTTI_VIRTUAL_CLASS(BaseResource, RTTIClass)
	// Make sure to create a virtual destructor to avoid memory leaks
	virtual ~BaseResource() override = default;

	virtual void Reload();

	virtual uint64 GetMemorySize() const = 0;

	[[nodiscard]] const String& GetFileName() const { return mFile; }

protected:
	// Make sure to call this base!
	virtual void LoadFromFile(const String& inFile)
	{
		mFile = inFile;
	}

	static String sReadFile(const String& inFile);

private:
	String mFile = {};
	int64 mFileLastModifiedTime = 0; ///< time_since_epoch

	friend class ResourceManager;
};
