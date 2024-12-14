#pragma once
#include <App/App.h>

#include <Game/GameApp.h>

class EditorApp : public App
{
public:
	EditorApp();

	virtual void Update(float inDeltaTime) override;

private:
	DC::UniquePtr<GameApp> mGameApp;
};
