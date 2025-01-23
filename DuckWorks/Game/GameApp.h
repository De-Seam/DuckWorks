#pragma once
// Core includes
#include <DuckCore/Containers/UniquePtr.h>

// App includes
#include <App/App.h>

class World;

class GameApp : public App
{
public:
	static void sRegisterRTTI();

	GameApp();
	virtual ~GameApp() override;

	virtual void Update(float inDeltaTime) override;

	World* GetWorld() { return mWorld; }

private:
	DC::UniquePtr<World> mWorld;
};
