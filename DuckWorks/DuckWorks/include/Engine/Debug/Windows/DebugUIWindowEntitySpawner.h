#pragma once
#include "DebugUIWindow.h"

class DebugUIWindowEntitySpawner : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntitySpawner, DebugUIWindow)

public:
	DebugUIWindowEntitySpawner();

	virtual void Update(float inDeltaTime) override;

private:
};
