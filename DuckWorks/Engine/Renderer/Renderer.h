#pragma once
// Engine includes
#include <DuckCore/Math/Rect.h>
#include <DuckCore/Math/Vector.h>

#include <Engine/Manager.h>

// External includes

namespace DC
{
struct Transform2D;
}

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;
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

	void SetWindowSize(DC::IVec2 inSize);

	[[nodiscard]]
	SDL_Texture* CreateTexture(DC::IVec2 inSize);
	void DestroyTexture(SDL_Texture*& ioTexture);

	void DrawTexture(SDL_Texture* inTexture,const DC::Transform2D& inTransform);
	void DrawSprite(const Sprite& inSprite, const DC::Transform2D& inTransform);
	void DrawRectangle(const DC::FRect2D& inRect, const DC::RGBA& inColor);
	void DrawRectangle(const DC::IRect2D& inRect, const DC::RGBA& inColor);

	// Class to automatically set the render target, and unset it when it leaves its scope
	class ScopedRenderTarget : public DC::NoCopy, public DC::NoMove
	{
	public:
		ScopedRenderTarget(SDL_Texture* inRenderTarget);
		~ScopedRenderTarget();

	private:
		SDL_Texture* mRenderTarget = nullptr;
		SDL_Texture* mPreviousRenderTarget = nullptr;
	};

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
};
