#pragma once
// Core includes
#include "UID.h"
#include "Utilities.h"
#include <Core/Allocators/ClassAllocator.h>
#include <Core/Utilities/GUID.h>


#pragma warning( push )
#pragma warning( disable : 4099) // First seen using 'class' now seen using 'struct'

#define RTTI_CLASS(inClassName, inParentClassName) \
public: \
	using Base = inParentClassName; \
\
	static const char* sGetClassName() { return #inClassName; } \
\
	static const char* sGetParentClassName() { return #inParentClassName; } \
\
	virtual const char* GetClassName() const override { return inClassName::sGetClassName(); } \
\
	virtual const char* GetParentClassName() const override { return inClassName::sGetParentClassName(); } \
\
	virtual Json Serialize() override; \
\
	virtual void Deserialize(const Json& inJson) override; \
\
	static const UID &sGetRTTIUID() { return s##inClassName##RTTIUID; } \
\
	virtual const UID &GetRTTIUID() const override { return s##inClassName##RTTIUID; } \
\
	virtual bool IsAUID(const UID& inRTTIUID) const override \
	{ \
		if (inRTTIUID == sGetRTTIUID()) \
			return true; \
		else \
			return Base::IsAUID(inRTTIUID); \
	} \
\
	template<typename taType> \
	static bool sIsA() \
	{ \
		return sIsAUID(taType::sGetRTTIUID()); \
	} \
\
	static bool sIsAUID(const UID& inRTTIUID) \
	{ \
	if(inRTTIUID == sGetRTTIUID()) \
		return true; \
	else \
		return Base::sIsAUID(inRTTIUID); \
	} \
\
	template<typename... taArgs> \
	static inClassName* sNewInstance(taArgs&&... inArgs) \
	{ \
		##inClassName* instance = s##inClassName##ClassAllocator.Allocate(ALLOC_TRACK); \
		instance = new (instance) inClassName(std::forward<taArgs>(inArgs)...); \
		return instance; \
	} \
\
	virtual void Delete() override \
	{ \
		this->~##inClassName##(); \
		s##inClassName##ClassAllocator.Free(this); \
	} \
\
	static ClassAllocator<##inClassName##>& sGetClassAllocator() { return s##inClassName##ClassAllocator; } \
\
	private: \
		static ClassAllocator<##inClassName##> s##inClassName##ClassAllocator; \
		static UID s##inClassName##RTTIUID; \
\
	public:

#define RTTI_CLASS_DEFINITION(inClassName) \
	UID inClassName::s##inClassName##RTTIUID; \
	ClassAllocator<inClassName> inClassName::s##inClassName##ClassAllocator; 

#define RTTI_EMPTY_SERIALIZE_DEFINITION(inClassName) \
	Json inClassName::Serialize() { return Base::Serialize(); } \
	void inClassName::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

/****
Order of initialization with RTTI:
1. Constructor
2. Init (Called by the factory)
3. Deserialize
*****/
class RTTIBaseClass
{
public:
	virtual ~RTTIBaseClass() = default;

	virtual const char* GetClassName() const = 0;
	virtual const char* GetParentClassName() const = 0;

	virtual Json Serialize();
	virtual void Deserialize(const Json& inJson);

	static const UID& sGetRTTIUID() { return sRTTIBaseClassRTTIUID; }
	virtual const UID& GetRTTIUID() const { return sRTTIBaseClassRTTIUID; }

	template<typename taType>
	static bool sIsA()
	{
		return sIsAUID(taType::sGetRTTIUID());
	}

	static bool sIsAUID(const UID& inRTTIUID)
	{
		return inRTTIUID == sGetRTTIUID();
	}

	template<typename taType>
	bool IsA() const
	{
		return IsAUID(taType::sGetRTTIUID());
	}

	virtual bool IsAUID(const UID& inRTTIUID) const
	{
		return inRTTIUID == sGetRTTIUID();
	}

	const UID& GetUID() const { return mUID; }

	template<typename... taArgs>
	static RTTIBaseClass* sNewInstance(taArgs&&... inArgs)
	{
		RTTIBaseClass* instance = sRTTIBaseClassClassAllocator.Allocate(ALLOC_TRACK);
		instance = new (instance) RTTIBaseClass(std::forward<taArgs>(inArgs)...);
		return instance;
	}

	virtual void Delete()
	{
		this->~RTTIBaseClass();
		sRTTIBaseClassClassAllocator.Free(this);
	}

	static ClassAllocator<RTTIBaseClass>& sGetClassAllocator() { return sRTTIBaseClassClassAllocator; }

	void SetGUID(const GUID& inGUID) { mGUID = inGUID; }
	const GUID& GetGUID() const { return mGUID; }

private:
	GUID mGUID = {};

	static ClassAllocator<RTTIBaseClass> sRTTIBaseClassClassAllocator;
	static UID sRTTIBaseClassRTTIUID;
	UID mUID = {};
};

// Dynamic casting
template<typename taCastType>
taCastType* gCast(RTTIBaseClass* inObject)
{
	if (inObject->IsA<taCastType>())
		return RCast<taCastType*>(inObject);
	return nullptr;
}

#define REGISTER_ENTITY(inEntity) \
	gEntityFactory.RegisterClass<inEntity>(#inEntity)

#define REGISTER_COMPONENT(inComponent) \
	gEntityComponentFactory.RegisterClass<inComponent>(#inComponent); 

#define REGISTER_DEBUG_UI_WINDOW(inWindow) \
	gDebugUIWindowFactory.RegisterClass<inWindow>(#inWindow)
