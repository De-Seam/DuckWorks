#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowEntitySpawner : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntitySpawner, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:
};

#endif
