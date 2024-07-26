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

bool RTTIClass::IsA(const RTTI& inRTTI) 
{
	return sGetRTTI().GetTypeID() == inRTTI.GetTypeID();
}

void RTTIClass::BroadcastMessage(MsgBase& ioMsg) 
{
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = mMessageListeners[ioMsg.GetMsgTypeID()];
	for (Pair<RTTIClass*, Function<void(MsgBase&)>>& listener : listeners) 
		listener.second(ioMsg);
}
