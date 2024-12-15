#include <DuckCore/Math/Random.h>
#include <DuckCore/Math/Transform.h>

#include <Engine/Events/SDLEventManager.h>

#include <Sandbox/SandboxApp.h>

#include <SDL/SDL_render.h>

SandboxApp::SandboxApp()
{
	Renderer& renderer = gEngine->GetManager<Renderer>();
	renderer.SetWindowSize({ cWindowWidth, cWindowHeight });
	mTexture = renderer.CreateTexture({ cWindowWidth, cWindowHeight });
}

SandboxApp::~SandboxApp() = default;

void SandboxApp::Update(float inDeltaTime)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();
	SDLEventManager& sdl_event_manager = gEngine->GetManager<SDLEventManager>();

	{
		Renderer::ScopedRenderTarget scoped_render_target(mTexture);

		for (int y = cWindowHeight-2; y >= 0; y--)
		{
			for (int x = cWindowWidth-1; x >= 0; x--)
			{
				Pixel& pixel = mPixels[y * cWindowWidth + x];

				if (!pixel.mFilled)
					continue;

				{
					DC::IVec2 lower_pixel_position = { x, y + 1 };
					Pixel& lower_pixel = mPixels[lower_pixel_position.mY * cWindowWidth + lower_pixel_position.mX];
					if (!lower_pixel.mFilled)
					{
						pixel.mFilled = false;
						SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 255);
						SDL_RenderDrawPoint(renderer.GetRenderer(), x, y);

						lower_pixel.mFilled = true;
						SDL_SetRenderDrawColor(renderer.GetRenderer(), 255, 255, 255, 255);
						SDL_RenderDrawPoint(renderer.GetRenderer(), lower_pixel_position.mX, lower_pixel_position.mY);

						continue;
					}
				}

				mRandomState = DC::gXorShift32(&mRandomState);
				if (mRandomState % 2 == 0)
				{
					if (x != 0)
					{
						DC::IVec2 lower_left_pixel_position = { x - 1, y + 1 };
						Pixel& lower_left_pixel = mPixels[lower_left_pixel_position.mY * cWindowWidth + lower_left_pixel_position.mX];
						if (!lower_left_pixel.mFilled)
						{
							pixel.mFilled = false;
							SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 255);
							SDL_RenderDrawPoint(renderer.GetRenderer(), x, y);

							lower_left_pixel.mFilled = true;
							SDL_SetRenderDrawColor(renderer.GetRenderer(), 255, 255, 255, 255);
							SDL_RenderDrawPoint(renderer.GetRenderer(), lower_left_pixel_position.mX, lower_left_pixel_position.mY);

							continue;
						}
					}
				}
				else
				{
					if (x != cWindowWidth - 1)
					{
						DC::IVec2 lower_right_pixel_position = { x + 1, y + 1 };
						Pixel& lower_right_pixel = mPixels[lower_right_pixel_position.mY * cWindowWidth + lower_right_pixel_position.mX];
						if (!lower_right_pixel.mFilled)
						{
							pixel.mFilled = false;
							SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 255);
							SDL_RenderDrawPoint(renderer.GetRenderer(), x, y);

							lower_right_pixel.mFilled = true;
							SDL_SetRenderDrawColor(renderer.GetRenderer(), 255, 255, 255, 255);
							SDL_RenderDrawPoint(renderer.GetRenderer(), lower_right_pixel_position.mX, lower_right_pixel_position.mY);

							continue;
						}
					}
				}
			}
		}

		if (sdl_event_manager.IsMouseButtonDown(EMouseButton::Left))
		{
			DC::IVec2 mouse_position = sdl_event_manager.GetMousePosition();

			if (mouse_position.mX >= 0 && mouse_position.mX < cWindowWidth && mouse_position.mY >= 0 && mouse_position.mY < cWindowHeight)
			{
				Pixel& pixel = mPixels[mouse_position.mY * cWindowWidth + mouse_position.mX];
				pixel.mFilled = !pixel.mFilled;

				if (pixel.mFilled)
					SDL_SetRenderDrawColor(renderer.GetRenderer(), 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 255);

				SDL_RenderDrawPoint(renderer.GetRenderer(), mouse_position.mX, mouse_position.mY);
			}
		}

		else if (sdl_event_manager.IsMouseButtonDown(EMouseButton::Right))
		{
			DC::IVec2 mouse_position = sdl_event_manager.GetMousePosition();

			if (mouse_position.mX >= 0 && mouse_position.mX < cWindowWidth && mouse_position.mY >= 0 && mouse_position.mY < cWindowHeight)
			{
				Pixel& pixel = mPixels[mouse_position.mY * cWindowWidth + mouse_position.mX];
				pixel.mFilled = !pixel.mFilled;

				if (pixel.mFilled)
					SDL_SetRenderDrawColor(renderer.GetRenderer(), 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 255);

				SDL_RenderDrawPoint(renderer.GetRenderer(), mouse_position.mX, mouse_position.mY);
			}
		}
	}

	static DC::Transform2D transform;
	transform.mPosition = {0, 0};
	transform.mHalfSize.mX = cWindowWidth / 2;
	transform.mHalfSize.mY = cWindowHeight / 2;

	renderer.DrawTexture(mTexture, transform);
}
