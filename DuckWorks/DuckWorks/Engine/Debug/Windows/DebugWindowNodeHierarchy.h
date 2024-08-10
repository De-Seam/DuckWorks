#pragma once
#include <Engine/Debug/Windows/DebugWindow.h>

class DebugWindowNodeHierarchy : public DebugWindow
{
	RTTI_CLASS(DebugWindowNodeHierarchy, DebugWindow)
public:
	virtual void Update(float inDeltaTime) override;
};