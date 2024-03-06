#pragma once
#include "Engine/Debug/Windows/DebugUIWindow.h"

class Entity;
class ComponentBase;

#define STRING_BUFFER_SIZE 256

class DebugUIWindowEntityDetails : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEntityDetails, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:
	WeakPtr<Entity> mSelectedEntity;

private:
	void HandleKeyValuePair(Json& inJson, const String& inComponentName, ComponentBase* inComponent, const String& inKey, Json& inValue,
							bool inSameLine = false);
};
