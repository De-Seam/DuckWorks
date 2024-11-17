#pragma once
// Core includes
#include <DuckCore/Containers/UniquePtr.h>

// App includes
#include <App/App.h>

class World;

class GameApp : public App
{
public:
	GameApp();
	virtual ~GameApp() override;

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update(float inDeltaTime) override;

private:
	DC::UniquePtr<World> mWorld;
};
