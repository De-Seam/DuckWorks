#pragma once
#include "Core/Utilities/RTTI.h"

struct BaseResource : public RTTIBaseClass
{
	RTTI_CLASS(BaseResource, RTTIBaseClass)
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	BaseResource(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	// Make sure to create a virtual destructor to avoid memory leaks
	virtual ~BaseResource() override {}

	// Make sure to call this base!
	virtual void LoadFromFile(const String& inFile)
	{
		mFile = inFile;
	}

	[[nodiscard]] const String& GetFileName() const { return mFile; }

private:
	String mFile = {};
};
