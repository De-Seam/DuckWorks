#pragma once
#include <Core/RTTI/RTTIRefObject.h>

class DebugWindow : public RTTIRefObject
{
	RTTI_CLASS(DebugWindow, RTTIRefObject)

public:
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	bool IsOpen() const { return mIsOpen; }

protected:
	bool mIsOpen = false;
};
