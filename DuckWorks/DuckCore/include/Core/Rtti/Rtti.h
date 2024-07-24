#pragma once
// Core includes
#include <Core/Utilities/TypeID.h>

// Std includes
#include <functional>

using RTTITypeID = TypeID<class RttiClass>;

class RTTIClass;

class RTTI
{
public:
	RTTI(
		const char* inClassName,
		const char* inBaseClassName,
		std::function<RTTIClass*()> inConstructorFunction
	) :
		mClassName(inClassName),
		mBaseClassName(inBaseClassName),
		mConstructorFunction(inConstructorFunction) {}

	// We don't want people to copy this class. Just use a pointer or refernece to it
	RTTI& operator=(const RTTI&) = delete;
	RTTI(const RTTI&) = delete;

	const char* GetClassName() const { return mClassName; }
	const char* GetBaseClassName() const { return mBaseClassName; }
	RTTIClass* NewInstance() const { return mConstructorFunction(); }

	const RTTITypeID& GetTypeID() const { return mTypeID; }

private:
	const char* mClassName = nullptr;
	const char* mBaseClassName = nullptr;
	std::function<RTTIClass*()> mConstructorFunction;

	RTTITypeID mTypeID;
};

class RTTIClass
{
public:
	virtual const RTTI& GetRTTI() const { return sRTTI; }
	static const RTTI& sGetRTTI() { return sRTTI; }
	static RTTIClass* sNewInstance() { return new RTTIClass; }

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
	inline static RTTI sRTTI = RTTI(
		"RTTIClass", // Class Name
		"NONE", // Base Class Name
		[]() { return sNewInstance(); }
	);
};

#define RTTI_CLASS(inClassName, inBaseClassName) \
private: \
	using Base = inBaseClassName; \
\
public: \
	virtual const RTTI& GetRTTI() const override { return sRTTI; } \
	const RTTI& sGetRTTI() const { return sRTTI; } \
	static inClassName* sNewInstance() { return new inClassName; } \
	virtual bool IsA(const RTTI& inRTTI) \
	{ \
		if (sGetRTTI().GetTypeID() == inRTTI.GetTypeID()) \
			return true; \
		return Base::IsA(inRTTI); \
	} \
\
private: \
	inline static RTTI sRTTI = RTTI( \
		#inClassName, \
		#inBaseClassName, \
		[]() { return sNewInstance(); } \
	); \