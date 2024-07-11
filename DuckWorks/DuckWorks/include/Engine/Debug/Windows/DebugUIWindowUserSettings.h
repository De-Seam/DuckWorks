#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowUserSettings : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowUserSettings, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	DebugUIWindowUserSettings(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;
};

#endif
