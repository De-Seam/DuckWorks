#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowUserSettings : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowUserSettings, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:

private:
};
