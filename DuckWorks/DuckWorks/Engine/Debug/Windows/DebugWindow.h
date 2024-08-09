#pragma once
#include <Core/RTTI/RTTIRefObject.h>

class DebugWindow : public RTTIRefObject
{
	RTTI_CLASS(DebugWindow, RTTIRefObject)

public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	bool IsOpen() const { return mIsOpen; }
	void SetOpen(bool inIsOpen) { mIsOpen = inIsOpen; }

protected:
	bool mIsOpen = false;
};

inline Json DebugWindow::Serialize() const
{
	Json json = RTTIRefObject::Serialize();
	JSON_SAVE(json, mIsOpen);
	return json;
}

inline void DebugWindow::Deserialize(const Json& inJson)
{
	RTTIRefObject::Deserialize(inJson);
	JSON_LOAD(inJson, mIsOpen);
}
