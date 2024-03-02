#pragma once
#include "Windows/DebugUIWindow.h"
#include "Core/CoreBase.h"

class DebugUIWindowManager
{
public:
	void Update(float inDeltaTime);

	template<typename taType>
	WeakPtr<DebugUIWindow> CreateWindow();
	WeakPtr<DebugUIWindow> AddWindow(SharedPtr<DebugUIWindow> inWindow);

	template<typename taType>
	void RemoveWindow();
	void RemoveWindow(const String& inWindowClassName);

private:
	Array<SharedPtr<DebugUIWindow>> mWindows;
};

extern DebugUIWindowManager gDebugUIWindowManager;

template<typename taType>
WeakPtr<DebugUIWindow> DebugUIWindowManager::CreateWindow()
{
	SharedPtr<taType> new_window = std::make_shared<taType>();
	return AddWindow(new_window);
}

template<typename taType>
void DebugUIWindowManager::RemoveWindow()
{
	RemoveWindow(taType::sGetClassName());
}
