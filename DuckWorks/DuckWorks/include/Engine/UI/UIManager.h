#pragma once
// Core includes
#include "Core/Manager/Manager.h"

class UIWidget;

class UIManager : public Manager
{
	RTTI_CLASS(UIManager, Manager, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	UIManager(const ConstructParameters& inParameters = {}) : Base(inParameters) { mRequiresUpdate = true; }

	virtual void Update(float inDeltaTime) override;
	void Draw();

private:
	Array<Ref<UIWidget>> mRootWindows;
};

extern UIManager gUIManager;
