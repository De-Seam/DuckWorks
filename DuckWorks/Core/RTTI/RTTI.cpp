#include <Core/RTTI/RTTI.h>

void RTTIClass::BroadcastMessage(MsgBase& ioMsg) 
{
	auto it = mMessageListeners.find(ioMsg.GetMsgTypeID());
	if (it == mMessageListeners.end())
		return;
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = it->second;
	for (Pair<RTTIClass*, Function<void(MsgBase&)>>& listener : listeners) 
		listener.second(ioMsg);
}
