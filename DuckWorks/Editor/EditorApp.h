#pragma once
#include <App/App.h>
#include <Editor/Menus/EditorMenu.h>

#include <Game/GameApp.h>

class EditorApp : public App
{
public:
	static constexpr const char* cEditorJsonPath = "Editor/Editor.json";

	EditorApp();
	virtual ~EditorApp() override;

	virtual void Update(float inDeltaTime) override;

	template<typename taType>
	taType& GetMenu();

	GameApp* GetGameApp() { return mGameApp; }

private:
	DC::UniquePtr<GameApp> mGameApp;
	SDL_Texture* mGameRenderTarget = nullptr;

	DC::Array<DC::UniquePtr<EditorMenu>> mMenus;
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
