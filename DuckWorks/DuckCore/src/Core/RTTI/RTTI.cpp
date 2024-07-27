#include <Core/Precomp.h>
#include <Core/RTTI/RTTI.h>

RTTI::RTTI(
	const char* inClassName, 
	const char* inBaseClassName, 
	std::function<RTTIClass* ()> inConstructorFunction)
	:
	mClassName(inClassName),
	mBaseClassName(inBaseClassName),
	mConstructorFunction(inConstructorFunction) 
{}

RTTIClass::~RTTIClass() 
{
#ifdef _ASSERTS_ENABLED
	// view values for loop
	for (const auto& [key, value] : mMessageListeners) 
		gAssert(value.empty() && "Message listeners are not empty");
#endif
}

bool RTTIClass::IsA(const RTTI& inRTTI) const
{
	return sGetRTTI().GetTypeID() == inRTTI.GetTypeID();
}

void RTTIClass::BroadcastMessage(MsgBase& ioMsg) 
{
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[ioMsg.GetMsgTypeID()];
	for (Pair<RTTIClass*, Function<void(MsgBase&)>>& listener : listeners) 
		listener.second(ioMsg);
}
