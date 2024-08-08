#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowEntityDetails : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityDetails, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:
};

#endif
