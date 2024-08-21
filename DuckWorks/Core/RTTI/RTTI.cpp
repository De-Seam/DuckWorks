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

Json RTTIClass::Serialize() const 
{
	Json json;
	json["ClassName"] = GetRTTI().GetClassName();
	json["mGUID"] = mGUID.ToString();
	return json;
}

void RTTIClass::Deserialize(const Json& inJson)
{
	gAssert(inJson["ClassName"] == GetRTTI().GetClassName());
	mGUID = inJson["mGUID"];
}

void RTTIClass::BroadcastMessage(MsgBase& ioMsg) 
{
	auto it = mMessageListeners.find(ioMsg.GetMsgTypeID());
	if (it == mMessageListeners.end())
		return;
	Array<Pair<RTTIClass*, Function<void(MsgBase&)>>>& listeners = it->second;
	for (Pair<RTTIClass*, Function<void(MsgBase&)>>& listener : listeners) 
		listener.second(ioMsg);
}
