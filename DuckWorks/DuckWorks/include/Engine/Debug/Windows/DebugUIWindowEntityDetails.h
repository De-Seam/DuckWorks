#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

class Entity;
class ComponentBase;

class DebugUIWindowEntityDetails : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityDetails, DebugUIWindow)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowEntityDetails(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:
	WeakPtr<Entity> mSelectedEntity;

private:
};
