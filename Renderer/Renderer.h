#pragma once
// Engine includes
#include <DuckCore/Events/Event.h>
#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/Manager.h>
#include <DuckCore/Math/Rect.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/Utilities/NoCopy.h>

#include <Renderer/RenderTarget.h>

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
	static void sStartup();

	Renderer();
	virtual ~Renderer() override;

	void BeginFrame();
	void EndFrame();

	void SetWindowSize(DC::IVec2 aSize);

	[[nodiscard]]
	SDL_Texture* CreateTexture(DC::IVec2 aSize);
	void DestroyTexture(SDL_Texture*& aTexture);

	DC::Ref<RenderTarget> CreateRenderTarget(DC::IVec2 aSize);
	static void sDestroyTexture(SDL_Texture*& aTexture); // Destroy a texture. This will set it to nullptr.

	void DrawTexture(SDL_Texture* aTexture,	const DC::Transform2D& aTransform);
	// void DrawSprite(const Sprite& aSprite, const DC::Transform2D& aTransform);
	void DrawRectangle(const DC::FRect2D& aRect, const DC::RGBA& aColor);
	void DrawRectangle(const DC::IRect2D& aRect, const DC::RGBA& aColor);
	void Clear(DC::RGBA aColor);

	// Class to automatically set the render target, and unset it when it leaves its scope.
	class ScopedRenderTarget : public DC::NoCopy, public DC::NoMove
	{
	public:
		explicit ScopedRenderTarget(RenderTarget& aRenderTarget);
		~ScopedRenderTarget();

	private:
		DC::Ref<RenderTarget> mRenderTarget;
		DC::Ref<RenderTarget> mPreviousRenderTarget;
	};

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	RenderTarget* GetRenderTarget() { return mRenderTarget; }
	const RenderTarget* GetRenderTarget() const { return mRenderTarget; }

private:
	void SetRenderTarget(RenderTarget* aRenderTarget);

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	DC::Ref<RenderTarget> mRenderTarget = nullptr;

	DC::UniquePtr<DC::EventHandle<DC::ShutdownEvent>> mShutdownEventHandle;

	friend class ScopedRenderTarget;
};
