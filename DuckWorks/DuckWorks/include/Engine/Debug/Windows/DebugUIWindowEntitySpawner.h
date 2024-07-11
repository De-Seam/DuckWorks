#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowEntitySpawner : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntitySpawner, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowEntitySpawner(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:
};

#endif
