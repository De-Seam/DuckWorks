#pragma once
// Engine includes
#include <Engine/Manager.h>

// External includes
#include <External/SDL/SDL.h>

namespace DC
{
struct Transform2D;
}

class Sprite;

class Renderer : public Manager
{
	RTTI_CLASS(Renderer, Manager)
public:
	Renderer();
	virtual ~Renderer() override;

	virtual void Init() override;
	virtual void Shutdown() override;

	void BeginFrame();
	void EndFrame();

	void DrawSprite(const Sprite& inSprite, const DC::Transform2D& inTransform);

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
};
