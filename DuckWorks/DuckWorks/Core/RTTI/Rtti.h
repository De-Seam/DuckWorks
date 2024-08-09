#pragma once
// Core includes
#include <Core/Config.h>
#include <Core/Containers/Function.h>
#include <Core/Containers/HashMap.h>
#include <Core/Containers/Pair.h>
#include <Core/RTTI/Message.h>
#include <Core/RTTI/RTTIInterface.h>
#include <Core/Utilities/Assert.h>
#include <Core/Utilities/GUID.h>
#include <Core/Utilities/Json.h>
#include <Core/Utilities/TypeID.h>
#include <Core/Utilities/Utilities.h>

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
	virtual ~RTTIClass();

	virtual const RTTI& GetRTTI() const { return sRTTI; }
	static const RTTI& sGetRTTI() { return sRTTI; }
	static RTTIClass* sNewInstance() { return new RTTIClass; }

	template<typename taType>
	bool IsA() const;
	virtual bool IsA(const RTTI& inRTTI) const;

	template<typename taType>
	taType* As();

	template<typename taType>
	taType* AsInterface();

	virtual bool ImplementsInterface(const RTTIInterfaceTypeID& inInterface) const { return false; }

	virtual Json Serialize() const;
	virtual void Deserialize(const Json& inJson);

	void BroadcastMessage(MsgBase& ioMsg);
	template<typename taType>
	void BroadcastMessage() { taType msg; BroadcastMessage(msg); }

	template<typename taRecipientClass, typename taMsgType>
	void RegisterMessageListener(taRecipientClass* inRecipient, void (taRecipientClass::*inFunction)(taMsgType&));

	template<typename taRecipientClass, typename taMsgType>
	void UnregisterMessageListener(taRecipientClass* inRecipient, void (taRecipientClass::*inFunction)(taMsgType&));

	void SetGUID(const GUID& inGUID) { mGUID = inGUID; }
	const GUID& GetGUID() const { return mGUID; }

private:
	inline static RTTI sRTTI = RTTI(
		"RTTIClass", // Class Name
		"NONE", // Base Class Name
		[]() { return reinterpret_cast<RTTIClass*>(sNewInstance()); }
	);

	GUID mGUID;

	HashMap<MsgTypeID, Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>> mMessageListeners;
};

template<typename taType>
bool RTTIClass::IsA() const 
{
	return IsA(taType::sGetRTTI());
}

template<typename taType>
taType* RTTIClass::As() 
{
	if (this->IsA(taType::sGetRTTI()))
		return static_cast<taType>(this);
	return nullptr;
}

template<typename taType>
taType* RTTIClass::AsInterface() 
{
	if (this->ImplementsInterface(taType::sInterfaceTypeID))
		return static_cast<taType*>(this);
	return nullptr;
}

template<typename taRecipientClass, typename taMsgType>
void RTTIClass::RegisterMessageListener(taRecipientClass* inRecipient, void(taRecipientClass::* inFunction)(taMsgType&)) 
{
#ifdef _ASSERTS_ENABLED
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[taMsgType::sGetMsgTypeID()];
	for (int32 i = static_cast<int32>(listeners.size()) - 1; i >= 0 ; i--) 
		gAssert(listeners[i].first != inRecipient && "Message listener already registered!");
#endif
	Function<void(MsgBase&)> function = [inRecipient, inFunction] (MsgBase& inMsg) 
	{
		(inRecipient->*inFunction)(*reinterpret_cast<taMsgType*>(&inMsg));
	};
	mMessageListeners[taMsgType::sGetMsgTypeID()].push_back(Pair(inRecipient, function));
}

template<typename taRecipientClass, typename taMsgType>
void RTTIClass::UnregisterMessageListener(taRecipientClass* inRecipient, void(taRecipientClass::* inFunction)(taMsgType&)) 
{
	(void)inFunction;
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[taMsgType::sGetMsgTypeID()];
	for (int32 i = static_cast<int32>(listeners.size()) - 1; i >= 0 ; i--) 
	{
		if (listeners[i].first == inRecipient)
		{
			gSwap(listeners[i], listeners.back());
			listeners.pop_back();
			return;
		}
	}
	gAssert(false);
}

// Template function for to_json
template<typename T>
typename std::enable_if<has_serialize<T, Json()>::value>::type
to_json(Json& j, const T& obj) 
{
    j = obj.Serialize();
}

// Template function for from_json
template<typename T>
typename std::enable_if<has_deserialize<T, void(const Json&)>::value>::type
from_json(const Json& j, T& obj) 
{
    obj.Deserialize(j);
}

template<typename T>
typename std::enable_if<has_serialize<T, Json()>::value>::type
to_json(Json& j, const T* obj) 
{
    j = obj->Serialize();
}

// Template function for from_json
template<typename T>
typename std::enable_if<has_deserialize<T, void(const Json&)>::value>::type
from_json(const Json& j, T* obj) 
{
    obj->Deserialize(j);
}

#define RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) \
private: \
	using Base = inBaseClassName; \
\
public: \
	virtual const RTTI& GetRTTI() const override { return sRTTI; } \
	static const RTTI& sGetRTTI() { return sRTTI; } \
	virtual bool IsA(const RTTI& inRTTI) const \
	{ \
		if (sGetRTTI().GetTypeID() == inRTTI.GetTypeID()) \
			return true; \
		return Base::IsA(inRTTI); \
	} \
	virtual bool ImplementsInterface(const RTTIInterfaceTypeID& inInterface) const override \
	{ \
		for (const RTTIInterfaceTypeID& interface : sInterfaces) \
		{ \
			if (interface == inInterface) \
				return true; \
		} \
		return Base::ImplementsInterface(inInterface); \
	} \
private: \
	inline static RTTI sRTTI = RTTI( \
		#inClassName, \
		#inBaseClassName, \
		[]() { return reinterpret_cast<RTTIClass*>(sNewInstance()); } \
	);

#define RTTI_VIRTUAL_CLASS_DECLARATION(inClassName, inBaseClassName) \
public: \
	static inClassName* sNewInstance() { gAssert(false); return nullptr; } \
RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) 

#define RTTI_CLASS_DECLARATION(inClassName, inBaseClassName) \
public: \
	static inClassName* sNewInstance() { return new inClassName; } \
RTTI_CLASS_DECLARATION_BASE(inClassName, inBaseClassName) 


#pragma warning( push )
#pragma warning( disable : 4002)
#pragma warning( disable : 4003)


#define EXPAND( x ) x

// The multiple argument rtti classes
#define RTTI_CLASS_2(A,B) \
inline static StaticArray<RTTIInterfaceTypeID, 0> sInterfaces = {}; \
RTTI_CLASS_DECLARATION(A, B) 

#define RTTI_CLASS_3(A,B,C) \
inline static StaticArray<RTTIInterfaceTypeID, 1> sInterfaces = { C::sInterfaceTypeID }; \
RTTI_CLASS_DECLARATION(A, B)

#define RTTI_CLASS_4(A,B,C,D) \
inline static StaticArray<RTTIInterfaceTypeID, 2> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID }; \
RTTI_CLASS_DECLARATION(A, B)

#define RTTI_CLASS_5(A,B,C,D,E) \
inline static StaticArray<RTTIInterfaceTypeID, 3> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID }; \
RTTI_CLASS_DECLARATION(A, B)

#define RTTI_CLASS_6(A,B,C,D,E,F) \
inline static StaticArray<RTTIInterfaceTypeID, 4> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID, F::sInterfaceTypeID }; \
RTTI_CLASS_DECLARATION(A, B)

#define RTTI_CLASS_7(A,B,C,D,E,F,G) \
inline StaticArray<RTTIInterfaceTypeID, 5> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID, F::sInterfaceTypeID, G::sInterfaceTypeID }; \
RTTI_CLASS_DECLARATION(A, B)

// The interim macro that simply strips the excess and ends up with the required macro
#define RTTI_CLASS_X(x,A,B,C,D,E,F,G,FUNC, ...)  FUNC  

// The macro that the programmer uses 
#define RTTI_CLASS(...)				EXPAND( RTTI_CLASS_X(,##__VA_ARGS__, \
										RTTI_CLASS_7(__VA_ARGS__), \
										RTTI_CLASS_6(__VA_ARGS__), \
										RTTI_CLASS_5(__VA_ARGS__), \
										RTTI_CLASS_4(__VA_ARGS__), \
										RTTI_CLASS_3(__VA_ARGS__), \
										RTTI_CLASS_2(__VA_ARGS__) \
									) ) 

// The multiple argument virtual rtti classes
#define RTTI_VIRTUAL_CLASS_2(A,B) \
inline static StaticArray<RTTIInterfaceTypeID, 0> sInterfaces = {}; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B) 

#define RTTI_VIRTUAL_CLASS_3(A,B,C) \
inline static StaticArray<RTTIInterfaceTypeID, 1> sInterfaces = { C::sInterfaceTypeID }; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B)

#define RTTI_VIRTUAL_CLASS_4(A,B,C,D) \
inline static StaticArray<RTTIInterfaceTypeID, 2> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID }; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B)

#define RTTI_VIRTUAL_CLASS_5(A,B,C,D,E) \
inline static StaticArray<RTTIInterfaceTypeID, 3> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID }; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B)

#define RTTI_VIRTUAL_CLASS_6(A,B,C,D,E,F) \
inline static StaticArray<RTTIInterfaceTypeID, 4> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID, F::sInterfaceTypeID }; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B)

#define RTTI_VIRTUAL_CLASS_7(A,B,C,D,E,F,G) \
inline static StaticArray<RTTIInterfaceTypeID, 5> sInterfaces = { C::sInterfaceTypeID, D::sInterfaceTypeID, E::sInterfaceTypeID, F::sInterfaceTypeID, G::sInterfaceTypeID }; \
RTTI_VIRTUAL_CLASS_DECLARATION(A, B)

// The interim macro that simply strips the excess and ends up with the required macro
#define RTTI_VIRTUAL_CLASS_X(x,A,B,C,D,E,F,G,FUNC, ...)  FUNC  

// The macro that the programmer uses 
#define RTTI_VIRTUAL_CLASS(...)			EXPAND( RTTI_VIRTUAL_CLASS_X(,##__VA_ARGS__, \
											RTTI_VIRTUAL_CLASS_7(__VA_ARGS__), \
											RTTI_VIRTUAL_CLASS_6(__VA_ARGS__), \
											RTTI_VIRTUAL_CLASS_5(__VA_ARGS__), \
											RTTI_VIRTUAL_CLASS_4(__VA_ARGS__), \
											RTTI_VIRTUAL_CLASS_3(__VA_ARGS__), \
											RTTI_VIRTUAL_CLASS_2(__VA_ARGS__) \
										) )

#pragma warning( pop )
