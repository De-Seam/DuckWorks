#pragma once
// Engine includes
#include <Engine/Manager.h>

// External includes
#include <External/SDL/SDL.h>

class Renderer : public Manager
{
	RTTI_CLASS(Renderer, Manager)
public:
	Renderer();
	virtual ~Renderer() override;

	virtual void Init();
	virtual void Shutdown();

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
};