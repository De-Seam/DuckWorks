#pragma once
#include "Windows/DebugUIWindow.h"
#include "Core/CoreBase.h"

class Entity;

class DebugUIWindowManager
{
public:
	void Init();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void Update(float inDeltaTime);
	void UpdateMainMenuBar();
	void UpdateViewport();
	void UpdateWindows(float inDeltaTime);

	template<typename taType>
	WeakPtr<DebugUIWindow> CreateWindow();
	WeakPtr<DebugUIWindow> AddWindow(SharedPtr<DebugUIWindow> inWindow);

	template<typename taType>
	[[nodiscard]]
	SharedPtr<DebugUIWindow> GetWindow() const;
	[[nodiscard]]
	SharedPtr<DebugUIWindow> GetWindow(const String& inWindowClassName) const;

	template<typename taType>
	bool WindowExists() const;
	bool WindowExists(const String& inWindowClassName) const;

	template<typename taType>
	void RemoveWindow();
	void RemoveWindow(const String& inWindowClassName);

	void SetSelectedEntity(const WeakPtr<Entity>& inEntity);
	WeakPtr<Entity> GetSelectedEntity() const { return mSelectedEntity; }

private:
	Array<SharedPtr<DebugUIWindow>> mWindows;
	Array<SharedPtr<DebugUIWindow>> mWindowsToAdd;

	Array<bool> mWindowOpen;
	String mDebugFileName = "Debug.json";

	WeakPtr<Entity> mSelectedEntity;
};

extern DebugUIWindowManager gDebugUIWindowManager;

template<typename taType>
WeakPtr<DebugUIWindow> DebugUIWindowManager::CreateWindow()
{
	SharedPtr<taType> new_window = std::make_shared<taType>();
	return AddWindow(new_window);
}

template<typename taType>
SharedPtr<DebugUIWindow> DebugUIWindowManager::GetWindow() const
{
	return GetWindow(taType::sGetClassName());
}

template<typename taType>
bool DebugUIWindowManager::WindowExists() const
{
	return WindowExists(taType::sGetClassName());
}

template<typename taType>
void DebugUIWindowManager::RemoveWindow()
{
	RemoveWindow(taType::sGetClassName());
}
