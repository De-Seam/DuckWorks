#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowImGuiExample : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowImGuiExample, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:

private:
};
