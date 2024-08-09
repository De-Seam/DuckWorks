#pragma once
#include <Engine/Manager/Manager.h>

#define STRING_BUFFER_SIZE 256

class DebugWindow;
struct MsgAnyWindowEvent;

class DebugManager : public Manager
{
	RTTI_MANAGER(DebugManager, Manager)
public:
	DebugManager();

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float inDeltaTime) override;

	// Assumes a imgui window is created.
	// Return true if the value was modified
	static bool sDebugDrawJson(Json& ioJson, const String& inLabel);

	// Return true if the value was modified
	static bool sHandleKeyValuePair(Json& ioJson, const String& inLabel, const String& inKey, Json& ioValue, bool inSameLine = false, bool inShowKey = true);

protected:
	void OnAnyWindowEvent(const MsgAnyWindowEvent& inMsg);

private:
	Array<Ref<DebugWindow>> mDebugWindows;
};
