#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowEntityList : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityList, DebugUIWindow)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowEntityList(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:
};

#endif
