#pragma once
// Core includes
#include <Core/Config.h>
#include <Core/Containers/Function.h>
#include <Core/Containers/Pair.h>
#include <Core/RTTI/Message.h>
#include <Core/Utilities/Assert.h>
#include <Core/Utilities/TypeID.h>

using RTTITypeID = TypeID<class RTTIClass>;

class RTTIClass;

// RTTI are static per RTTI type. They are used to identify the type of a class
class RTTI
{
public:
	RTTI(
		const char* inClassName,
		const char* inBaseClassName,
		std::function<RTTIClass*()> inConstructorFunction
	);

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

	virtual bool IsA(const RTTI& inRTTI);

	template<typename taType>
	taType* As();

	void BroadcastMessage(MsgBase& ioMsg);

	template<typename taRecipientClass, typename taMsgType>
	void RegisterMessageListener(taRecipientClass* inRecipient, void (taRecipientClass::*inFunction)(taMsgType&));

	template<typename taRecipientClass, typename taMsgType>
	void UnregisterMessageListener(taRecipientClass* inRecipient, void (taRecipientClass::*inFunction)(taMsgType&));

private:
	inline static RTTI sRTTI = RTTI(
		"RTTIClass", // Class Name
		"NONE", // Base Class Name
		[]() { return reinterpret_cast<RTTIClass*>(sNewInstance()); }
	);

	HashMap<MsgTypeID, Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>> mMessageListeners;
};

template<typename taType>
taType* RTTIClass::As() 
{
	if (this->IsA(taType::sGetRTTI()))
		return static_cast<taType>(this);
	return nullptr;
}

template<typename taRecipientClass, typename taMsgType>
void RTTIClass::RegisterMessageListener(taRecipientClass* inRecipient, void(taRecipientClass::* inFunction)(taMsgType&)) 
{
#ifdef _ASSERTS_ENABLED
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[taMsgType::sGetMsgTypeID()];
	for (int32 i = listeners.size() - 1; i >= 0 ; i--) 
		gAssert(listeners[i].first != inRecipient && "Message listener already registered!");
#endif
	Function<void(MsgBase&)> function = [inRecipient, inFunction] (MsgBase& inMsg) 
	{
		(inRecipient->*inFunction)(static_cast<taMsgType&>(inMsg));
	};
	mMessageListeners[taMsgType::sGetMsgTypeID()].push_back(function);
}

template<typename taRecipientClass, typename taMsgType>
void RTTIClass::UnregisterMessageListener(taRecipientClass* inRecipient, void(taRecipientClass::* inFunction)(taMsgType&)) 
{
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[taMsgType::sGetMsgTypeID()];
	for (int32 i = listeners.size() - 1; i >= 0 ; i--) 
	{
		if (listeners[i].first == inRecipient)
		{
			listeners.erase(listeners.begin() + i);
			return;
		}
	}
	gAssert(false);
}

#define RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) \
private: \
	using Base = inBaseClassName; \
\
public: \
	virtual const RTTI& GetRTTI() const override { return sRTTI; } \
	static const RTTI& sGetRTTI() { return sRTTI; } \
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
		[]() { return reinterpret_cast<RTTIClass*>(sNewInstance()); } \
	);

#define RTTI_VIRTUAL_CLASS(inClassName, inBaseClassName) \
RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) \
public: \
	static inClassName* sNewInstance() { gAssert(false); return nullptr; } \
private: \

#define RTTI_CLASS(inClassName, inBaseClassName) \
RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) \
public: \
	static inClassName* sNewInstance() { return new inClassName; } \
private: \
