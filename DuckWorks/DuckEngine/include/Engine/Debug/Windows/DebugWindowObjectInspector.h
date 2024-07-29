#pragma once
#include <Engine/Debug/Windows/DebugWindow.h>

class DebugWindowObjectInspector : public DebugWindow
{
	RTTI_CLASS(DebugWindowObjectInspector, DebugWindow)
public:
	virtual void Update(float inDeltaTime) override;
};