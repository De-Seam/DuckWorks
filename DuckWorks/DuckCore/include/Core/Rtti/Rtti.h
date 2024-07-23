#pragma once
#include <Core/Utilities/TypeId.h>

class RttiClass;
using RttiTypeId = TypeId<RttiClass>;

class Rtti
{
public:
	Rtti(
		const char* inClassName,
		const char* inBaseClassName,
		RttiTypeId inTypeId
	) :
		mClassName(inClassName),
		mBaseClassName(inBaseClassName),
		mTypeId(inTypeId) {}

	const char* GetClassName() const { return mClassName; }
	const char* GetBaseClassName() const { return mBaseClassName; }

private:
	const char* mClassName = nullptr;
	const char* mBaseClassName = nullptr;
	RttiTypeId mTypeId = -1;
};

class RttiClass
{
public:

private:
	static Rtti* mRtti = nullptr;
};
