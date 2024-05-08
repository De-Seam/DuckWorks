#pragma once
#include "UID.h"
#include "Utilities.h"

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
	private: \
		static UID s##inClassName##RTTIUID; \
\
	public:

#define RTTI_CLASS_DEFINITION(inClassName) \
	UID inClassName::s##inClassName##RTTIUID;

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
	virtual void Deserialize(const Json& inJson) { (void)inJson; }

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

private:
	static UID sRTTIBaseClassRTTIUID;
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
	gComponentFactory.RegisterClass<inComponent>(#inComponent); \
	gEntityComponentFactory.RegisterClass<inComponent>(#inComponent); \
	gEntityComponentManager.RegisterComponentType<inComponent>()

#define REGISTER_DEBUG_UI_WINDOW(inWindow) \
	gDebugUIWindowFactory.RegisterClass<inWindow>(#inWindow)
