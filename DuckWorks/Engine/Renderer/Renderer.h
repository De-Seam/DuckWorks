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
