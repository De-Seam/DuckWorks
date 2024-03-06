#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

class DebugUIWindowEntityList : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityList, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:
};
