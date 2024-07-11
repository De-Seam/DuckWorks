#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowImGuiExample : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowImGuiExample, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowImGuiExample(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;
};

#endif
