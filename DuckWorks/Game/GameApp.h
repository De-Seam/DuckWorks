#pragma once
#include <DuckCore/RTTI/Ref.h>

#include <DuckWorks/App/App.h>

class World;

class GameApp : public App
{
public:
	GameApp();
	virtual ~GameApp() override;

	virtual void Update(float inDeltaTime) override;

	 World* GetWorld() { return mWorld; }

private:
	DC::Ref<World> mWorld;
};
