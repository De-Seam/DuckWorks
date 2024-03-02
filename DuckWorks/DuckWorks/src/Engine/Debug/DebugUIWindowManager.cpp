#include "Precomp.h"
#include "Engine/Debug/DebugUIWindowManager.h"

#include "Engine/Debug/Windows/DebugUIWindow.h"

DebugUIWindowManager gDebugUIWindowManager = {};

void DebugUIWindowManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowManager::Update)

	Array<SharedPtr<DebugUIWindow>> windows_to_remove;
	for (SharedPtr<DebugUIWindow>& window : mWindows)
	{
		window->Update(inDeltaTime);
		if (!window->IsOpen())
			windows_to_remove.push_back(window);
	}
	for (const SharedPtr<DebugUIWindow>& window : windows_to_remove)
	{
		RemoveWindow(window->GetClassName());
	}
}

WeakPtr<DebugUIWindow> DebugUIWindowManager::AddWindow(SharedPtr<DebugUIWindow> inWindow)
{
	for (SharedPtr<DebugUIWindow>& window : mWindows)
	{
		if (window == inWindow || window->GetClassName() == inWindow->GetClassName())
		{
			gAssert(false, "Duplicate window already exists! If this is a mistake, check that RTTI_CLASS is defined.");
			return {};
		}
	}

	mWindows.push_back(inWindow);
	return inWindow;
}

void DebugUIWindowManager::RemoveWindow(const String& inWindowClassName)
{
	for (uint64 i = 0; i < mWindows.size(); i++)
	{
		if (mWindows[i]->GetClassName() == inWindowClassName)
		{
			mWindows.erase(mWindows.begin() + i);
			return;
		}
	}
}
