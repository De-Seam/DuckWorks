#pragma once
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/Math/Vector.h>

#include <Editor/Menus/EditorMenu.h>

#include <Game/GameApp.h>

struct SDL_Texture;

class ViewportMenu : public EditorMenu
{
	RTTI_CLASS(ViewportMenu, EditorMenu)
public:
	ViewportMenu(EditorApp& inEditorApp);

	virtual void Update(float inDeltaTime) override;

	GameApp* GetGameApp() { return mGameApp; }

private:
	DC::UniquePtr<GameApp> mGameApp;
	SDL_Texture* mGameRenderTarget = nullptr;
	DC::UVec2 mGameRenderTargetSize = { 800, 600 };
};
