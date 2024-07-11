#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowEntityDetails : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityDetails, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	DebugUIWindowEntityDetails(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:
};

#endif
