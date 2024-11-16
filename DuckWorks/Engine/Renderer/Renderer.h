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

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
};