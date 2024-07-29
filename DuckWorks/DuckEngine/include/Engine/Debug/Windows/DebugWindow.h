#pragma once
#include <Core/RTTI/RTTIRefObject.h>

class DebugWindow : public RTTIRefObject
{
	RTTI_CLASS(DebugWindow, RTTIRefObject)

public:
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	bool IsOpen() const { return mIsOpen; }
	void SetOpen(bool inIsOpen) { mIsOpen = inIsOpen; }

protected:
	bool mIsOpen = false;
};
