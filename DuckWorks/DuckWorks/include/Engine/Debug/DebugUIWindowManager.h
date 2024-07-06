#pragma once
// Core includes
#include "Core/CoreBase.h"

// Engine includes
#include "Windows/DebugUIWindow.h"

class TextureRenderComponent;
class Entity;

class DebugUIWindowManager : public Manager
{
	RTTI_CLASS(DebugUIWindowManager, Manager, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	DebugUIWindowManager(const ConstructParameters& inConstructParameters = {})
		: Base(inConstructParameters)
	{
		// The DebugUIWindowManager needs to be updated manually at a specific point, so we disable the automatic updates
		mRequiresUpdate = false;
	}

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void Update(float inDeltaTime) override;

	void BeginFrame();
	void EndFrame();

	void UpdateViewport();
	void UpdateSelectedEntity();

	void UpdateMainMenuBar();
	void UpdateMainMenuBarWindowMenu();
	void UpdateMainMenuBarDrawModes();

	void UpdateWindows(float inDeltaTime);

	template<typename taType>
	WeakRef<DebugUIWindow> CreateWindow(const typename taType::ConstructParameters& inConstructParameters = {});
	WeakRef<DebugUIWindow> AddWindow(Ref<DebugUIWindow> inWindow);

	template<typename taType>
	[[nodiscard]]
	Ref<taType> GetWindow() const;
	[[nodiscard]]
	Ref<DebugUIWindow> GetWindow(const String& inWindowClassName) const;

	template<typename taType>
	bool WindowExists() const;
	bool WindowExists(const String& inWindowClassName) const;

	template<typename taType>
	void RemoveWindow();
	void RemoveWindow(const String& inWindowClassName);

	void SetSelectedEntity(const Optional<WeakRef<Entity>>& inEntity);
	Optional<WeakRef<Entity>> GetSelectedEntity() const { return mSelectedEntity; }

public:
	bool mDrawEntityOutline = true;
	bool mDrawCollision = false;
	bool mDrawBVH = false;

private:
	Array<Ref<DebugUIWindow>> mWindows;
	Array<Ref<DebugUIWindow>> mWindowsToAdd;

	Array<bool> mWindowOpen;
	String mDebugFileName = "Debug.json";

	Optional<WeakRef<Entity>> mSelectedEntity;
	TextureRenderComponent* mSelectedTextureRenderComponent;
	Vec2 mSelectedTextureRenderComponentRelativeLocation = {}; ///< 0,0 would be the center of the texture render component

	HashMap<UID, Json> mSavedWindowSettings; ///< The saved settings for each Window.
};

extern DebugUIWindowManager gDebugUIWindowManager;

template<typename taType>
WeakRef<DebugUIWindow> DebugUIWindowManager::CreateWindow(const typename taType::ConstructParameters& inConstructParameters)
{
	Ref<taType> new_window = Ref<taType>{inConstructParameters};
	return AddWindow(new_window);
}

template<typename taType>
Ref<taType> DebugUIWindowManager::GetWindow() const
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
