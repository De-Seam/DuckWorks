#pragma once
#include <App/App.h>

#include <Editor/EditorSettings.h>
#include <Editor/Menus/EditorMenu.h>

#include <Game/GameApp.h>

class EditorApp : public App
{
public:
	static void sRegisterRTTI();

	static constexpr const char* cEditorJsonPath = "Editor/Editor.json";

	EditorApp();
	virtual ~EditorApp() override;

	virtual void Update(float inDeltaTime) override;

	template<typename taType>
	taType& GetMenu();

	const EditorSettings& GetEditorSettings() const { return *mEditorSettings; }
	EditorSettings& GetEditorSettings() { return *mEditorSettings; }

private:
	DC::Array<DC::UniquePtr<EditorMenu>> mMenus;

	DC::UniquePtr<EditorSettings> mEditorSettings;
};

template <typename taType>
taType& EditorApp::GetMenu()
{
	for (EditorMenu* menu : mMenus)
	{
		if (menu->IsA<taType>())
			return menu->Cast<taType>();
	}
	gAssert(false);
	abort();
}
