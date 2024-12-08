#pragma once
#include <App/App.h>

#include <Game/GameApp.h>

class EditorApp : public App
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update(float inDeltaTime) override;

private:
	DC::UniquePtr<GameApp> mGameApp;
};
