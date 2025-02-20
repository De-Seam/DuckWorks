#pragma once
// Engine includes
#include <DuckCore/Manager/Manager.h>
#include <DuckCore/Math/Rect.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/Utilities/NoCopy.h>

namespace DC
{
struct Transform2D;
}

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;
class Sprite;

class Renderer : public DC::Manager
{
	MANAGER_BASE_CLASS(Renderer)
public:
	Renderer();
	virtual ~Renderer() override;

	void BeginFrame();
	void EndFrame();

	void SetWindowSize(DC::IVec2 aSize);

	[[nodiscard]]
	SDL_Texture* CreateTexture(DC::IVec2 aSize);
	void DestroyTexture(SDL_Texture*& aTexture);

	void DrawTexture(SDL_Texture* aTexture,	const DC::Transform2D& aTransform);
	void DrawSprite(const Sprite& aSprite, const DC::Transform2D& aTransform);
	void DrawRectangle(const DC::FRect2D& aRect, const DC::RGBA& aColor);
	void DrawRectangle(const DC::IRect2D& aRect, const DC::RGBA& aColor);

	// Class to automatically set the render target, and unset it when it leaves its scope
	class ScopedRenderTarget : public DC::NoCopy, public DC::NoMove
	{
	public:
		ScopedRenderTarget(SDL_Texture* aRenderTarget);
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
