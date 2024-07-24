#pragma once
#include <Core/Utilities/TypeID.h>

using RTTITypeID = TypeID<class RttiClass>;

class RTTI
{
public:
	RTTI(
		const char* inClassName,
		const char* inBaseClassName
	) :
		mClassName(inClassName),
		mBaseClassName(inBaseClassName) {}

	// We don't want people to copy this class. Just use a pointer or refernece to it
	RTTI& operator=(const RTTI&) = delete;
	RTTI(const RTTI&) = delete;

	const char* GetClassName() const { return mClassName; }
	const char* GetBaseClassName() const { return mBaseClassName; }
	const RTTITypeID& GetTypeID() const { return mTypeID; }

private:
	const char* mClassName = nullptr;
	const char* mBaseClassName = nullptr;
	RTTITypeID mTypeID;
};

class RTTIClass
{
private:
	inline static RTTI sRTTI = RTTI(
		"RTTIClass", // Class Name
		"NONE" // Base Class Name
		);

public:
	virtual const RTTI& GetRTTI() const { return sRTTI; }
	const RTTI& sGetRTTI() const { return sRTTI; }

	virtual bool IsA(const RTTI& inRTTI)
	{
		return sGetRTTI().GetTypeID() == inRTTI.GetTypeID();
	}

	template<typename taType>
	taType* As()
	{
		if (this->IsA(taType::sGetRTTI()))
			return static_cast<taType>(this);
		return nullptr;
	}
private:
};

#define RTTI_CLASS(inClassName, inBaseClassName) \
private: \
	inline static RTTI sRTTI = RTTI( \
		#inClassName, \
		#inBaseClassName \
	); \
	using Base = inBaseClassName; \
\
public: \
	virtual const RTTI& GetRTTI() const override { return sRTTI; } \
	const RTTI& sGetRTTI() const { return sRTTI; } \
	virtual bool IsA(const RTTI& inRTTI) \
	{ \
		if (sGetRTTI().GetTypeID() == inRTTI.GetTypeID()) \
			return true; \
		return Base::IsA(inRTTI); \
	} \
\
private: