#pragma once
// Core includes
#include <Core/Allocators/ClassAllocator.h>
#include <Core/Allocators/StandardAllocator.h>
#include <Core/Utilities/GUID.h>
#include "Core/Utilities/UID.h"
#include "Core/Utilities/Utilities.h"

#pragma warning( push )
#pragma warning( disable : 4099) // First seen using 'class' now seen using 'struct'

#define RTTI_VIRTUAL_CLASS(inClassName, inParentClassName)                                                                                                     \
public:                                                                                                                                                        \
	using Base = inParentClassName;                                                                                                                            \
                                                                                                                                                               \
	static const char *sGetClassName() { return #inClassName; }                                                                                                \
                                                                                                                                                               \
	static const char *sGetParentClassName() { return #inParentClassName; }                                                                                    \
                                                                                                                                                               \
	virtual const char *GetClassName() const override { return inClassName::sGetClassName(); }                                                                 \
                                                                                                                                                               \
	virtual const char *GetParentClassName() const override { return inClassName::sGetParentClassName(); }                                                     \
                                                                                                                                                               \
	virtual Json Serialize() override;                                                                                                                         \
                                                                                                                                                               \
	virtual void Deserialize(const Json &inJson) override;                                                                                                     \
                                                                                                                                                               \
	static const UID &sGetRTTIUID() { return s##inClassName##RTTIUID; }                                                                                        \
                                                                                                                                                               \
	virtual const UID &GetRTTIUID() const override { return s##inClassName##RTTIUID; }                                                                         \
                                                                                                                                                               \
	virtual bool IsAUID(const UID &inRTTIUID) const override                                                                                                   \
	{                                                                                                                                                          \
		if (inRTTIUID == sGetRTTIUID())                                                                                                                        \
			return true;                                                                                                                                       \
		else                                                                                                                                                   \
			return Base::IsAUID(inRTTIUID);                                                                                                                    \
	}                                                                                                                                                          \
                                                                                                                                                               \
	template<typename taType>                                                                                                                                  \
	static bool sIsA()                                                                                                                                         \
	{                                                                                                                                                          \
		return sIsAUID(taType::sGetRTTIUID());                                                                                                                 \
	}                                                                                                                                                          \
                                                                                                                                                               \
	static bool sIsAUID(const UID &inRTTIUID)                                                                                                                  \
	{                                                                                                                                                          \
		if (inRTTIUID == sGetRTTIUID())                                                                                                                        \
			return true;                                                                                                                                       \
		else                                                                                                                                                   \
			return Base::sIsAUID(inRTTIUID);                                                                                                                   \
	}                                                                                                                                                          \
                                                                                                                                                               \
private:                                                                                                                                                       \
	static UID s##inClassName##RTTIUID;                                                                                                                        \
                                                                                                                                                               \
public:

#define RTTI_CLASS(inClassName, inParentClassName, inAllocatorClass)                                                                                                             \
	RTTI_VIRTUAL_CLASS(inClassName, inParentClassName)																										   \
                                                                                                                                                               \
	template<typename... taArgs>                                                                                                                               \
	static inClassName* sNewInstance(taArgs&&... inArgs)                                                                                                       \
	{                                                                                                                                                          \
		##inClassName* instance = s##inClassName##Allocator.Allocate(ALLOC_TRACK);                                                                        \
		instance = new (instance) inClassName(std::forward<taArgs>(inArgs)...);                                                                                \
		return instance;                                                                                                                                       \
	}                                                                                                                                                          \
                                                                                                                                                               \
	virtual void Delete() override                                                                                                                             \
	{                                                                                                                                                          \
		this->~##inClassName##();                                                                                                                              \
		s##inClassName##Allocator.Free(this);                                                                                                             \
	}                                                                                                                                                          \
                                                                                                                                                               \
	static inAllocatorClass<##inClassName##>& sGetAllocator() { return s##inClassName##Allocator; }                                                    \
                                                                                                                                                               \
private:                                                                                                                                                       \
	static inAllocatorClass<##inClassName##> s##inClassName##Allocator;                                                                                     \
                                                                                                                                                               \
public:

#define RTTI_VIRTUAL_CLASS_DEFINITION(inClassName) \
	UID inClassName::s##inClassName##RTTIUID;

#define RTTI_CLASS_DEFINITION(inClassName, inAllocatorClass) \
	RTTI_VIRTUAL_CLASS_DEFINITION(inClassName) \
	inAllocatorClass<##inClassName##> inClassName::s##inClassName##Allocator;

#define RTTI_EMPTY_SERIALIZE_DEFINITION(inClassName) \
	Json inClassName::Serialize() { return Base::Serialize(); } \
	void inClassName::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

class MsgBase;

/****
Order of initialization with RTTI:
1. Constructor
2. Init (Called by the factory)
3. Deserialize
*****/
class RTTIBaseClass
{
public:
	// ConstructParameters need to be inherited. All variables need to have default values for RTTI
	struct ConstructParameters
	{
		GUID mGUID = {};
	};

	RTTIBaseClass(const ConstructParameters& inConstructParameters) : mGUID(inConstructParameters.mGUID) {}

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

	virtual void Delete()
	{
		gAssert(false, "Delete not implemented for this class");
	}

	void SetGUID(const GUID& inGUID) { mGUID = inGUID; }
	const GUID& GetGUID() const { return mGUID; }

	template<typename taRecipientClass, typename taMsgType>
	void RegisterMessageListener(taRecipientClass* inRecipient, void (taRecipientClass::*inFunction)(taMsgType&))
	{
		static_assert(std::is_base_of(MsgBase, taMsgType), "taMsgType must be a subclass of MsgBase");
		static_assert(std::is_base_of(RTTIBaseClass, taRecipientClass), "taRecipientClass must be a subclass of RTTIBaseClass");

		Function<void(MsgBase&)> function = [&](MsgBase& ioMsg)
		{
			(*inRecipient.*(inFunction))(*SCast<taMsgType*>(&ioMsg));
		};
		
		mMessages[taMsgType::sGetRTTIUID()].emplace_back(Pair(inRecipient, function));
	}

	template<typename taMsgType>
	void UnregisterMessageListener(RTTIBaseClass* inRecipient)
	{
		Array<Pair<RTTIBaseClass*, Function<void(MsgBase&)>>>& messages = mMessages[taMsgType::sGetRTTIUID()];
		for (int32 i = messages.size() - 1; i >= 0; i--)
		{
			if (messages[i].first == inRecipient)
				messages.erase(i);
		}
	}

	template<typename taMsgType>
	void SendMessage(taMsgType& ioMsg)
	{
		for (Pair<RTTIBaseClass*, Function<void(MsgBase&)>>& message : mMessages[taMsgType::sGetRTTIUID()])
		{
			message.second(ioMsg);
		}
	}

private:
	GUID mGUID = {};

	static UID sRTTIBaseClassRTTIUID;
	UID mUID = {};

	// Map [Msg UID : Array< Pair<recipient of callbacks, function> >]
	HashMap<UID, Array<Pair<RTTIBaseClass*, Function<void(MsgBase&)>>>> mMessages;
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

#define REGISTER_DEBUG_UI_WINDOW(inWindow) \
	gDebugUIWindowFactory.RegisterClass<inWindow>(#inWindow)

#define REGISTER_RTTI_CLASS(inRTTIClass) \
	gRTTIFactory.RegisterClass<inRTTIClass>(#inRTTIClass)
