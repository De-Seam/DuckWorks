#pragma once
#include "DebugUIWindow.h"

class DebugUIWindowUserSettings : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowUserSettings, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowUserSettings(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:

private:
};
