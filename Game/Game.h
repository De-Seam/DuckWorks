#pragma once
#include <Engine/Objects/Object.h>

class World;

class Game : public Object
{
	RTTI_CLASS(Game, Object)
public:
	Game();
	virtual ~Game() override;

	void Update(float aDeltaTime);
	void Render();

private:
	DC::Ref<World> mWorld;
};
