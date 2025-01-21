#include <DuckCore/Math/Intersects.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Renderer/Renderer.h>

#include <Game/UI/UIButton.h>

using namespace DC;

void UIButton::Update(float inDeltaTime)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();

	RGBA color = mColor;
	SDLEventManager& event_manager = gEngine->GetManager<SDLEventManager>();
	const IVec2& mouse_position = event_manager.GetMousePosition();
	if (gIntersects(mouse_position, mRectangle))
		color = event_manager.IsMouseButtonDown(EMouseButton::Left) ? mPressedColor : mHoverColor;

	renderer.DrawRectangle(mRectangle, color);

	Base::Update(inDeltaTime);
}
