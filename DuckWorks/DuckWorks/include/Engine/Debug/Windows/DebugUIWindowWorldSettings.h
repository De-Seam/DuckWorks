#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowWorldSettings : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowWorldSettings, DebugUIWindow)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowWorldSettings(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:

private:
};

#endif
